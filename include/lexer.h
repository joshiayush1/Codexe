#include"../include/headers.h"

enum class TokenType{
    KEYWORD,
    IDENTIFIER,
    ADD_OPERATOR,
    SUB_OPERATOR,
    MUL_OPERATOR,
    DIV_OPERATOR,
    NUMBER,
    ASSIGNMENT,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    SEMICOLON,
    STRING,
    UNKNOWN,
};
    
struct Token{
    TokenType type;
    string value;
    int line;
};


vector<Token> tokens;