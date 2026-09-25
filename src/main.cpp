// To run the codexe, you dont have to write lexer.cpp because tokenize is the function
// coded there. It will run because it literally copies entire lexer.cpp in main.cpp. 
// Each c++ files should be compiled seprately
// think .h files as menu of the restaurant that these things are available 
// here main.cpp will know that there is a function tokenize() in lexer.h

// to run codexe, 
// WRITE 'g++ main.cpp lexer.cpp -o main' in terminal in src
// g++ is c++ compiler.
// than -o creates object files for these cpp files i.e. main.o and lexer.o.
// now linker sees there is tokenize() in main.o and then it looks into other object files
// and finds lexer.o contains tokenize() function, then linker connects them and creates main.exe
// WRITE './main'

#include "../include/headers.h"
#include "../include/parser.h"

int main()
{
    tokenize();

    Parser parser;
    parser.ParseProgram();

    return 0;
}