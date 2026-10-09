#pragma once
#include "headers.h"

enum class ASTType{
    IDENTIFIER,
    ASSIGNMENT,
    NUMBER,
    ADD_OPERATOR,
    SUB_OPERATOR,
    MUL_OPERATOR,
    DIV_OPERATOR,
    LEFT_PAREN,
    RIGHT_PAREN,
    SEMICOLON,
    UNKNOWN,
};

class ASTNode{
    public:
    string data;
    ASTType type;
    ASTNode *left;
    ASTNode *right;

    ASTNode(string val, ASTType type){
        data = val;
        this -> type = type;
        left = right = nullptr;
    }
    ASTNode(string val,ASTType type, ASTNode *left, ASTNode *right){
        data = val;
        this -> type = type;
        this -> left = left;
        this -> right = right;
    }
};