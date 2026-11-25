// Ex. 1:
// declare result : int = 5 + 2 * 3;

// post-order traversal
//                  +
//              5       *
//                    2    3

// t1 = 2*3
// t2 = 5 + t1
// result = t2 i.e. 11

// Ex. 2:
// declare result : int = 5 * 2 + 3;

// post-order traversal
//                  *
//              5       +
//                    2    3

// t1 = 5 * 2
// t2 = t1 + 3
// result = t2 i.e. 13

// The important rule is:

// Whenever you reach an operator, its left and right children must already have been processed.

#include "../include/ir.h"

IR::IR()
{
    tempCounter = 0;
    labelCounter = 0;
}

vector<string> IR::getInstructions(){
    return instructions;
}

string IR::tempCount()
{
    tempCounter++;

    return "t" + to_string(tempCounter);
}

string IR::labelCount()
{
    labelCounter++;

    return "L" + to_string(labelCounter);
}

string IR::generateExpressionIR(ASTNode *node)
{
    if (node == nullptr)
    {
        return "";
    }

    if (node->left == nullptr && node->right == nullptr)
    {
        return node->data;
    }

    string left = generateExpressionIR(node->left);
    string right = generateExpressionIR(node->right);

    string temp = tempCount();

    string result = temp + " = " + left + " " + node->data + " " + right;

    instructions.push_back(result);

    return temp;
}

void IR::printAST(ASTNode *node)
{
    if (node == nullptr)
    {
        return;
    }

    printAST(node->left);
    printAST(node->right);

    cout << "Data : " << node->data << endl;
}

void IR::generateIfIR(ASTNode *node)
{
    ASTNode *comparisonNode = node->left;

    string comparisonOp = comparisonNode->data;

    string leftExp = generateExpressionIR(comparisonNode->left);
    string rightExp = generateExpressionIR(comparisonNode->right);

    ASTNode *if_block = node->right->left;
    ASTNode *else_block = node->right->right;

    string ifLabel = labelCount();
    string elseLabel;
    if (else_block != nullptr)
    {
        elseLabel = labelCount();
    }
    string endLabel = labelCount();

    instructions.push_back(
        "if " + leftExp + " " + comparisonOp + " " +
        rightExp + " goto " + ifLabel);

    if (else_block != nullptr)
    {
        instructions.push_back("goto " + elseLabel);
    }
    else
    {
        instructions.push_back("goto " + endLabel);
    }

    instructions.push_back(ifLabel + ":");

    IRGenerator(if_block->statements);

    if (else_block != nullptr)
    {
        instructions.push_back("goto " + endLabel);
        instructions.push_back(elseLabel + ":");

        IRGenerator(else_block->statements);
    }

    instructions.push_back(endLabel + ":");
}

void IR::generateForIR(ASTNode *node)
{
    ASTNode *withinNode = node->right;
    ASTNode *for_block = withinNode->right;

    ASTNode *iteratorNode = node->left->left;
    string iterator = iteratorNode->data;

    ASTNode *initialValueNode = node->left->right;
    string initialValue = generateExpressionIR(initialValueNode);

    instructions.push_back(iterator + " = " + initialValue);

    string startLabel = labelCount();
    string endLabel = labelCount();

    instructions.push_back(startLabel + ":");

    ASTNode *limitNode = withinNode->left;
    string limitValue = generateExpressionIR(limitNode);

    string op = ">=";

    instructions.push_back(
        "if " + iterator + " " + op + " " +
        limitValue + " goto " + endLabel);

    if (for_block != nullptr)
    {
        IRGenerator(for_block->statements);
    }

    string temp = tempCount();

    instructions.push_back(temp + " = " + iterator + " + 1");
    instructions.push_back(iterator + " = " + temp);

    instructions.push_back("goto " + startLabel);

    instructions.push_back(endLabel + ":");
}

void IR::generateWhileIR(ASTNode *node)
{
    ASTNode *comparisonNode = node->left;
    ASTNode *while_block = node->right;

    string startLabel = labelCount();
    string endLabel = labelCount();

    instructions.push_back(startLabel + ":");

    string leftExp = generateExpressionIR(comparisonNode->left);
    string rightExp = generateExpressionIR(comparisonNode->right);

    string op = comparisonNode->data;

    if (op == ">")
        op = "<=";
    else if (op == "<")
        op = ">=";
    else if (op == ">=")
        op = "<";
    else if (op == "<=")
        op = ">";
    else if (op == "==")
        op = "!=";
    else if (op == "!=")
        op = "==";

    instructions.push_back(
        "if " + leftExp + " " + op + " " +
        rightExp + " goto " + endLabel);

    if (while_block != nullptr)
    {
        IRGenerator(while_block->statements);
    }

    instructions.push_back("goto " + startLabel);

    instructions.push_back(endLabel + ":");
}

void IR::IRGenerator(vector<ASTNode *> statements)
{
    for (auto node : statements)
    {
        if (node == nullptr)
        {
            continue;
        }

        if (node->type == ASTType::DECLARATION)
        {
            if (node->right == nullptr)
            {
                continue;
            }

            string result = generateExpressionIR(node->right);

            instructions.push_back(node->left->data + " = " + result);
        }

        else if (node->type == ASTType::ASSIGNMENT)
        {
            string result = generateExpressionIR(node->right);

            instructions.push_back(node->left->data + " = " + result);
        }

        else if (node->type == ASTType::SHOW)
        {
            string result = generateExpressionIR(node->left);

            instructions.push_back(node->data + " " + result);
        }

        else if (node->type == ASTType::IF)
        {
            generateIfIR(node);
        }

        else if (node->type == ASTType::FOR)
        {
            generateForIR(node);
        }

        else if (node->type == ASTType::WHILE)
        {
            generateWhileIR(node);
        }
    }
}
void IR::printIR()
{
    cout << "Program IR:" << endl;
    for (string instruction : instructions)
    {
        cout << instruction << endl;
    }
}
