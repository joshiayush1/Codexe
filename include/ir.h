#pragma once

#include "../include/headers.h"
#include "../include/ast.h"
#include "../include/lexer.h"

class IR{
    private:
    int tempCounter;
    int labelCounter;

    string tempCount();
    string labelCount();

    public:
    IR();

    vector<string> instructions;
    
    string generateExpressionIR(ASTNode* node);
    void printAST(ASTNode* node);

    void IRGenerator(vector<ASTNode*> statements);
    void generateIfIR(ASTNode* node);
    void generateForIR(ASTNode* node);
    void generateWhileIR(ASTNode* node);
};