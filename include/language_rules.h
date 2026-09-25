#include "../include/headers.h"

unordered_map<string, TokenType> mpp;  //assigning key according to token type so we directly use map for accessing

void InitializerRules(){
    mpp["int"] = TokenType::KEYWORD;   
    mpp["print"] = TokenType::KEYWORD;   
    mpp["if"] = TokenType::KEYWORD;
    mpp["else"] = TokenType::KEYWORD;
    mpp["for"] = TokenType::KEYWORD;
    mpp["while"] = TokenType::KEYWORD;

    mpp["="] = TokenType::ASSIGNMENT;

    mpp["+"] = TokenType::ADD_OPERATOR;
    mpp["-"] = TokenType::SUB_OPERATOR;
    mpp["*"] = TokenType::MUL_OPERATOR;
    mpp["/"] = TokenType::DIV_OPERATOR;

    mpp["("] = TokenType::LEFT_PAREN;
    mpp[")"] = TokenType::RIGHT_PAREN;

    mpp["{"] = TokenType::LEFT_BRACE;
    mpp["}"] = TokenType::RIGHT_BRACE;

    mpp[";"] = TokenType::SEMICOLON;
}

TokenType getTokenType(string word){
    auto it = mpp.find(word);
    if(it != mpp.end()){
        return it->second;
    }
    else{
        return TokenType::UNKNOWN;
    }
}