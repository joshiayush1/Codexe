#pragma once

#include"../include/headers.h"

enum class TokenType{
    KEYWORD,
    IDENTIFIER,
    STRING,
    NUMBER,
    
    IF,
    ELSE,
    
    ASSIGNMENT,
    GREATER_THAN,
    LESS_THAN,
    GREATER_EQUAL,
    LESS_EQUAL,
    EQUAL_EQUAL,
    NOT_EQUAL,
    
    ADD_OPERATOR,
    SUB_OPERATOR,
    MUL_OPERATOR,
    DIV_OPERATOR,
    
    
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    
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
