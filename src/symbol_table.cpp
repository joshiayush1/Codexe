#include "../include/symbol_table.h"


void SymbolTable::addSymbol(string name, string type){
    symbolMap[name] = type;
}

void SymbolTable::printSymbolType(string name){
    cout << "Type for "<< name << " is: " << symbolMap[name] << endl;
}

bool SymbolTable::isPresent(string name){
   return symbolMap.find(name) != symbolMap.end();

    // Returns true if the variable is found because
    // find() returns an iterator pointing to its position,
    // which is not equal to end().

    // Returns false if the variable is not found because
    // find() returns end(), making both sides equal.
}