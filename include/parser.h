#pragma once

#include "../include/headers.h"
#include "../include/lexer.h"
#include "../include/ast.h"
#include "../include/ir.h"

class Parser{
    private:
    int current;
    vector<ASTNode*> programAST;

    public:
    Parser();
    
    Token currentToken();
    void advance();
    void printTokenData();
    void printNodeData(ASTNode *node);
    void deleteNode(ASTNode *node);
    bool isOperand();
    bool isAddSubOperator();
    bool isMulDivOperator();
    bool isComparisonOperator();

    ASTNode *ParseDeclaration(); // Keyword -> Identifier -> Assignment -> Expression -> Semicolon
    ASTNode *ParseFactor(); // Operand or Parenthesis
    ASTNode *ParseTerm(); // Factor -> * or / -> Factor
    ASTNode *ParseExpression(); // Term -> + or - -> Term
    ASTNode *ParseConditionals(); // Term -> + or - -> Term
    ASTNode *ParseForLoop(); // Term -> + or - -> Term
    ASTNode *ParseWhileLoop(); // Term -> + or - -> Term
    ASTNode * ParsePrint(); // Print -> Left parenthesis -> Identifier -> Right parenthesis -> Semicolon
    ASTNode * ParseAssignment(); // Identifier -> Assignment -> Number -> Semicolon

    ASTNode * ParseStatement();
    void ParseProgram();

    vector<ASTNode*> getAST();
};