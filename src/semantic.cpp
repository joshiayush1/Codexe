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
    if (st.isPresentinCurrentScope(name))
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
        if (leftType == "int" && rightType == "int")
        {
            return "int";
        }
        return "";
    }
    return "";
}

bool Semantic::checkConditionalErrors(
    ASTNode *leftExpression,
    ASTNode *rightExpression,
    ASTNode *comparisonNode,
    SymbolTable &st)
{
    string leftType = getExpressionType(leftExpression, st);
    string rightType = getExpressionType(rightExpression, st);

    string op = comparisonNode->data;

    // Equality and inequality
    if (op == "==" || op == "!=")
    {
        if (leftType != rightType)
        {
            cout << "Semantic Error: Cannot compare different data types" << endl;
            return true;
        }

        return false;
    }

    // Relational operators
    if (op == "<" || op == ">" ||
        op == "<=" || op == ">=")
    {
        if (leftType != "int" || rightType != "int")
        {
            cout << "Semantic Error: Relational operators require integer operands" << endl;
            return true;
        }

        return false;
    }

    cout << "Semantic Error: Invalid comparison operator" << endl;
    return true;
}