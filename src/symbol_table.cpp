#include "../include/symbol_table.h"


void SymbolTable::addSymbols(){
    symbolMap["int"] = "1";
    symbolMap["string"] = "Rahul";

    cout << symbolMap["int"];
    cout << symbolMap["string"];
}