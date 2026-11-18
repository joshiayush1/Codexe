#pragma once

#include "../include/lexer.h"
#include "../include/ast.h"

class SymbolTable{
    private:
    vector<unordered_map<string, string>> symbolMap;
    stack<int> scopeNo;

    public:
    SymbolTable();

    void enterScope();
    void exitScope();
    void addSymbol(string name, string type);
    void printSymbolType(string name);
    bool isPresent(string name);
    bool isPresentinCurrentScope(string name);
    string getSymbolType(string name);
};