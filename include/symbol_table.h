#pragma once

#include "../include/lexer.h"

class SymbolTable{
    private:
    unordered_map<string, string> symbolMap;

    public:
    void addSymbol(string name, string type);
    void printSymbolType(string name);
    bool isPresent(string name);
    string getSymbolType(string name);
};