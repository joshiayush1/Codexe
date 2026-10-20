#include "../include/semantic.h"

bool Semantic::checkVariable(string name, SymbolTable &st)
{
    if (st.isPresent(name))
    {
        return true;
    }
    cout << "Error: Variable '" << name << "' is not declared" << endl;
    return false;
}

bool Semantic::reDeclaration(string name, SymbolTable &st)
{
    if (st.isPresent(name))
    {
        cout << "Error: Redeclarataion of '" << name << "'" << endl;
        return true;
    }
    return false;
}

string Semantic::getExpressionType(ASTNode *node, SymbolTable &st)
{
    if (node == nullptr)
    {
        return "";
    }

    if (node->type == ASTType::NUMBER)
    {
        return "int";
    }

    if (node->type == ASTType::STRING)
    {
        return "string";
    }

    if (node->type == ASTType::IDENTIFIER)
    {
        return st.getSymbolType(node->data);
    }

    if (node->type == ASTType::ADD_OPERATOR ||
        node->type == ASTType::SUB_OPERATOR ||
        node->type == ASTType::MUL_OPERATOR ||
        node->type == ASTType::DIV_OPERATOR)
    {
        string leftType = getExpressionType(node->left, st);
        string rightType = getExpressionType(node->right, st);
        if(leftType == "int" && rightType == "int"){
            return "int";
        }
        return "";
    }
    return "";
}