#include "../include/optimizer.h"

string Optimizer::constantFolding(string instruction, int equalIndex, int operatorIndex)
{
    int n = instruction.size();
    string leftOperand; 
    string rightOperand;
    string result;

    for(int i=equalIndex + 1; i<operatorIndex; i++){
        if(instruction[i] != ' '){
            leftOperand += instruction[i];
        }
    }
    for(int i=operatorIndex + 1; i<n; i++){
        if(instruction[i] != ' '){
            rightOperand += instruction[i];
        }
    }

    int leftConstant;
    int rightConstant;
    size_t pos1;
    size_t pos2;
    try{
        leftConstant = stoi(leftOperand, &pos1);
        rightConstant = stoi(rightOperand, &pos2);
    }
    catch(invalid_argument){
        cout << "Error: Invalid operand. Cannot perform constant folding" << endl;
        return instruction;
    }
    catch(out_of_range){
        cout << "Error: Constant value is out of integer range" << endl;
        return instruction;
    }

    if(pos1 != leftOperand.size() || pos2 != rightOperand.size()){
        cout << "Warning: Non-constant operand. Skipping constant folding." << endl;
    }

    int operatorValue = instruction[operatorIndex];
    long long ans = 0;

    if(operatorValue == '/' && rightConstant == 0){
        cout << "Error: Division by zero. Skipping constant folding" << endl;
        return instruction;
    }

    cout << leftConstant << " " << operatorValue << " " << rightConstant <<endl;

    if(operatorValue == '+'){
        ans = leftConstant + rightConstant;
        if(ans > INT_MAX || ans < INT_MIN){
            cout << "Error: Arithmetic overflow. Skipping constant folding." << endl;
            return instruction;
        }
    }
    else if(operatorValue == '-'){
        ans = leftConstant - rightConstant;
        if(ans > INT_MAX || ans < INT_MIN){
            cout << "Error: Arithmetic overflow. Skipping constant folding." << endl;
            return instruction;
        }
    }
    else if(operatorValue == '*'){
        ans = leftConstant * rightConstant;
        if(ans > INT_MAX || ans < INT_MIN){
            cout << "Error: Arithmetic overflow. Skipping constant folding." << endl;
            return instruction;
        }
    }
    else if(operatorValue == '/'){
        ans = leftConstant / rightConstant;
        if(ans > INT_MAX || ans < INT_MIN){
            cout << "Error: Arithmetic overflow. Skipping constant folding." << endl;
            return instruction;
        }
    }

    for(int i=0; i<=equalIndex; i++){
        result.push_back(instruction[i]);
    }

    result.push_back(' ');

    string ansString = to_string(ans);
    for(int i=0; i<ansString.size(); i++){
        result.push_back(ansString[i]);
    }

    cout << result << endl;
    return result;
}

void Optimizer::optimize(vector<string> instructions)
{
    for (string instruction : instructions)
    {
        int equalIndex = string::npos;
        int operatorIndex = string::npos;
        
        equalIndex = instruction.find("=");

        if(instruction.find("+") != string::npos){
            operatorIndex = instruction.find("+");
        }
        else if(instruction.find("*") != string::npos){
            operatorIndex = instruction.find("*");
        }
        else if(instruction.find("-") != string::npos){
            operatorIndex = instruction.find("-");
        }
        else if(instruction.find("/") != string::npos){
            operatorIndex = instruction.find("/");
        }

        if(equalIndex != string::npos && operatorIndex != string::npos){
            string result = constantFolding(instruction, equalIndex, operatorIndex);
            optimizedInstruction.push_back(result);
            continue;
        }
        
        optimizedInstruction.push_back(instruction);

        equalIndex = string::npos;
        operatorIndex = string::npos;
    }
}
