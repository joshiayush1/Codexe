#include "../include/ir.h"

IR::IR()
{
    tempCounter = 0;
    labelCounter = 0;
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

    cout << temp << " = " << left << " " << node->data << " " << right << endl;

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

    string label1 = labelCount();
    string label2 = labelCount();
    string endLabel = labelCount();

    if (else_block == nullptr)
    {
        endLabel = label2;
    }

    cout << node->data << " " << leftExp << " " << comparisonOp << " " << rightExp << " goto " << label1 << endl;

    cout << "goto " << label2 << endl;

    cout << label1 << ":" << endl;
    IRGenerator(if_block->statements);
    if (else_block != nullptr)
    {
        cout << "goto " << endLabel << endl;
    }

    if (else_block != nullptr)
    {
        cout << label2 << ":" << endl;
        IRGenerator(else_block->statements);
    }

    cout << endLabel << ":" << endl;
}

void IR::generateForIR(ASTNode *node)
{

}

void IR::generateWhileIR(ASTNode *node)
{
    ASTNode *comparisonNode = node->left;
    ASTNode *while_block = node->right;
    
    string startLabel = labelCount();
    string endLabel = labelCount();
    
    cout << startLabel << ":" << endl;
    
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
    
    
    cout << "if" << " " << leftExp << " " << op << " " << rightExp << " goto " << endLabel << endl;

    if (while_block != nullptr)
    {
        IRGenerator(while_block->statements);
    }

    cout << "goto " << startLabel << endl;
    cout << endLabel << ":" << endl;
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
            { // just declared, nothing assigned
                continue;
            }
            cout << "DECLARATION IR:" << endl;
            string result = generateExpressionIR(node->right);
            cout << node->left->data << " = " << result << endl;
        }

        else if (node->type == ASTType::ASSIGNMENT)
        {
            cout << "ASSIGNMENT IR:" << endl;
            string result = generateExpressionIR(node->right);
            cout << node->left->data << " = " << result << endl;
        }

        else if (node->type == ASTType::SHOW)
        {
            cout << "PRINT IR:" << endl;
            string result = generateExpressionIR(node->left);
            cout << node->data << " " << result << endl;
        }

        else if (node->type == ASTType::IF)
        {
            cout << "IF IR:" << endl;
            generateIfIR(node);
        }

        else if (node->type == ASTType::FOR)
        {
            cout << "FOR IR:" << endl;
            generateForIR(node);
        }

        else if (node->type == ASTType::WHILE)
        {
            cout << "WHILE IR:" << endl;
            generateWhileIR(node);
        }
    }
}