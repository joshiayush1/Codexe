#pragma once

#include "../include/headers.h"
#include "../include/symbol_table.h"
#include "../include/ast.h"
#include "../include/semantic.h"

class Semantic{
    public:
    bool checkVariable(string name, SymbolTable &st);
    bool reDeclaration(string name, SymbolTable &st);
    string getExpressionType(ASTNode *node, SymbolTable &st);
    bool checkConditionalErrors(ASTNode* leftExp, ASTNode* rightExp, ASTNode* compNode, SymbolTable &st);
};