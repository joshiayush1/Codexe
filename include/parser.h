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
    bool ParseDeclaration(); // Keyword -> Identifier -> Assignment -> Number -> Semicolon
    bool ParseExpression(); // Number -> Operator -> Number -> Semicolon
    bool ParsePrint(); // Left parenthesis -> Print -> Right parenthesis -> Semicolon
    bool ParseAssignment(); // Identifier -> Assignment -> Number -> Semicolon

    bool ParseStatement();
    void ParseProgram();
};