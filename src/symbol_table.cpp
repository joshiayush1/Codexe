#include "../include/symbol_table.h"

SymbolTable::SymbolTable(){
    symbolMap.push_back({});
    scopeNo.push(0);
}

void SymbolTable::enterScope()
{
    int newScope = symbolMap.size();
    
    symbolMap.push_back({});
    scopeNo.push(newScope);
}

void SymbolTable::exitScope()
{
    if (scopeNo.size() > 1)
    {
        scopeNo.pop();
    }
}

void SymbolTable::addSymbol(string name, string type){
    symbolMap[scopeNo.top()][name] = type;
}

string SymbolTable::getSymbolType(string name){
    stack<int> temp = scopeNo;

    while(!temp.empty()){
        auto it = symbolMap[temp.top()].find(name);
        if(it != symbolMap[temp.top()].end()){
            return it->second;
        }
        temp.pop();
    }
    return "";
}

void SymbolTable::printSymbolType(string name){
    string type = getSymbolType(name);

    if(type != ""){
        cout << "Type for "<< name << " is: " << type << endl;
    }
    else{
        cout << "Error: Variable name not found" << endl;
    }
}

bool SymbolTable::isPresent(string name){
    stack<int> temp = scopeNo;

    while(!temp.empty()){
        if(symbolMap[temp.top()].find(name) != symbolMap[temp.top()].end()){
            return true;
        }
        temp.pop();
    }
    return false;
}

bool SymbolTable::isPresentinCurrentScope(string name){
    return (symbolMap[scopeNo.top()].find(name) != symbolMap[scopeNo.top()].end());
}
