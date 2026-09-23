#include "../include/headers.h"
#include "../include/lexer.h"
#include "../include/language_rules.h"

bool isIdentifier(string word){

    if(word.empty()){
        return false;
    }

    if(isdigit(word[0])){
        return false;
    }

    for(char ch : word){
        if(!isalpha(ch) && !isdigit(ch) && ch != '_'){
            return false;
        }
    }

    return true;
}

bool isNumber(string word){
    if(word.empty()){
        return false;
    }   
    for(char ch : word){
        if(ch < '0' || ch > '9'){
            return false;
        }
    }
    return true;
}

int main(){
    fstream file("../input/demo.cod");

    Token token;

    InitializerRules();

    string word;
    char ch;
    int line = 1;

    if(!file.is_open()){
        // cout << "Error opening file! " << endl;
    }
    
    while(file.get(ch)){
        TokenType symbolType = getTokenType(string(1, ch));
        if(isalpha(ch) || isdigit(ch) || ch == '_'){
            word += ch; 
        }
        else{
            if(!word.empty()){
                TokenType type = getTokenType(word);
                
                if(type == TokenType::KEYWORD){
                    // cout << "Keyword : " << word << endl;
                    token.type = type;
                    token.value = word;
                    token.line = line;

                    tokens.push_back(token);
                }
                else if(isNumber(word)){
                    // cout << "Number : " << word << endl;
                    token.type = TokenType::NUMBER;
                    token.value = word;
                    token.line = line;

                    tokens.push_back(token);
                }
                else if(isIdentifier(word)){
                    // cout << "Identifier : " << word << endl;
                    token.type = TokenType::IDENTIFIER;
                    token.value = word;
                    token.line = line;

                    tokens.push_back(token);
                }
                else{
                    // cout << "Error" << endl;            
                }
                word = "";
            }
            
            //Whitespace and NewLine
            // if(isspace(ch)){
            //     // cout << "Whitespace" << endl;
            //     continue;
            // }
            
            //NewLine
            if(ch == '\n'){
                // cout << "Whitespace" << endl;
                line++;
            }

            //Operators
            else if(symbolType == TokenType::ASSIGNMENT){
                // cout << "ASSIGNMENT : " << ch << endl;
                token.type = symbolType;
                    token.value = ch;
                    token.line = line;

                    tokens.push_back(token);
            }
            else if(symbolType == TokenType::ADD_OPERATOR){
                // cout << "ADD_OPERATOR : " << ch << endl;
                token.type = symbolType;
                    token.value = ch;
                    token.line = line;

                    tokens.push_back(token);
            }
            else if(symbolType == TokenType::SUB_OPERATOR){
                // cout << "SUB_OPERATOR : " << ch << endl;
                token.type = symbolType;
                    token.value = ch;
                    token.line = line;

                    tokens.push_back(token);
            }
            else if(symbolType == TokenType::MUL_OPERATOR){
                // cout << "MUL_OPERATOR : " << ch << endl;
                token.type = symbolType;
                    token.value = ch;
                    token.line = line;

                    tokens.push_back(token);
            }
            else if(symbolType == TokenType::DIV_OPERATOR){
                // cout << "DIV_OPERATOR : " << ch << endl;
                token.type = symbolType;
                    token.value = ch;
                    token.line = line;

                    tokens.push_back(token);
            }
            
            //Parentheses
            else if(symbolType == TokenType::LEFT_PAREN){
                // cout << "LEFT_PAREN : " << ch << endl;
                token.type = symbolType;
                    token.value = ch;
                    token.line = line;

                    tokens.push_back(token);
            }
            else if(symbolType == TokenType::RIGHT_PAREN){
                // cout << "RIGHT_PAREN : " << ch << endl;
                token.type = symbolType;
                    token.value = ch;
                    token.line = line;

                    tokens.push_back(token);
            }
            else if(symbolType == TokenType::LEFT_BRACE){
                // cout << "LEFT_BRACE : " << ch << endl;
                token.type = symbolType;
                    token.value = ch;
                    token.line = line;

                    tokens.push_back(token);
            }
            else if(symbolType == TokenType::RIGHT_BRACE){
                // cout << "RIGHT_BRACE : " << ch << endl;
                token.type = symbolType;
                    token.value = ch;
                    token.line = line;

                    tokens.push_back(token);
            }
            
            //SemiColon
            else if(symbolType == TokenType::SEMICOLON){
                // cout << "SEMICOLON : " << ch << endl;
                token.type = symbolType;
                    token.value = ch;
                    token.line = line;

                    tokens.push_back(token);
            }
            
            else{
                // cout << "UNKOWN token" << endl;
            }
        }
    }
    for(int i = 0; i < tokens.size(); i++){
    cout << "Type: " << static_cast<int>(tokens[i].type) << endl;
    cout << "Value: " << tokens[i].value << endl;
    cout << "Line: " << tokens[i].line << endl;
    }
}