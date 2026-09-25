#pragma once

#include"../include/headers.h"

enum class TokenType{
    KEYWORD,
    IDENTIFIER,
    ASSIGNMENT,
    NUMBER,
    ADD_OPERATOR,
    SUB_OPERATOR,
    MUL_OPERATOR,
    DIV_OPERATOR,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    STRING,
    SEMICOLON,
    UNKNOWN,
};
    
struct Token{
    TokenType type;
    string value;
    int line;
};


extern vector<Token> tokens; //writing it extern means compiler will just know it exists, its not declared globally.

void tokenize();

string TokenTypeToString(TokenType type);