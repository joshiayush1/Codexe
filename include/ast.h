#pragma once

#include "headers.h"

enum class ASTType{
    DECLARATION,
    PRINT,
    STRING,
    IDENTIFIER,
    NUMBER,
    IF,
    IF_ELSE,
    ELSE,
    BLOCK,
    COMPARISON_OPERATOR,
    
    ADD_OPERATOR,
    SUB_OPERATOR,
    MUL_OPERATOR,
    DIV_OPERATOR,
    
    ASSIGNMENT,
    UNKNOWN,
};

class ASTNode{
    public:
    string data;
    ASTType type;
    ASTNode *left;
    ASTNode *right;

    vector<ASTNode*> statements;

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
