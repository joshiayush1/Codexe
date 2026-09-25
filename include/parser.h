#pragma once

#include "../include/headers.h"
#include "../include/lexer.h"

class Parser{
    private:
    int current = 0;

    public:
    Parser();
    
    Token currentToken();
    void advance();
    bool ParseDeclaration();
    bool ParseExpression(); // Number -> Operator -> Number
    bool ParsePrint(); // Number -> Operator -> Number

    bool ParseStatement();
    void ParseProgram();
};