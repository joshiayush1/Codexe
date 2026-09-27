#pragma once

#include "../include/headers.h"
#include "../include/lexer.h"

class Parser{
    private:
    int current;

    public:
    Parser();
    
    Token currentToken();
    void advance();
    void printTokenData();
    bool isOperand();
    bool isAddSubOperator();
    bool isMulDivOperator();

    bool ParseDeclaration(); // Keyword -> Identifier -> Assignment -> Expression -> Semicolon
    bool ParseFactor(); // Operand or Parenthesis
    bool ParseTerm(); // Factor -> * or / -> Factor
    bool ParseExpression(); // Term -> + or - -> Term
    bool ParsePrint(); // Print -> Left parenthesis -> Identifier -> Right parenthesis -> Semicolon
    bool ParseAssignment(); // Identifier -> Assignment -> Number -> Semicolon

    bool ParseStatement();
    void ParseProgram();
};