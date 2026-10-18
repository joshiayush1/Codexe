#include "../include/headers.h"
#include "../include/lexer.h"

class SymbolTable{
    private:
    unordered_map<string, string> symbolMap;

    public:
    void addSymbols(string name, string type);
};