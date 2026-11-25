Compiler using C++!

Codexe

*For declaration - 

Syntax:
declare age : int = 25

For understanding:
(declare age variable : that is of integer datatype = which is 25)

1. Define the codexe language

- Define rules for codexe
Define what are the rules of codexe language
Like int is a keyword
     age is a identifier



2. Lexer (Lexical Analyzer)
(KEYWORD, NUMBER, IDENTIFIER, OPERATOR, STRING) is our token type.

- Create tokens in order to share it to parser.

The code first is received as text, charcter by charcter.
Then tokens are created using that text.

-Why token?
Imagine if we give text to parser.

int age = 18;

Parser will look it as i n t a g e = 1 8 ;(text) which makes no sense
Rather we send (int)Keyword (age)Identifier (=)Assignment (18)Number (;) semicolon 
This makes complete sense for further grammar analysis.

- Token Storage
Storing tokens in vector, for further processing



3. Parser
Its checks the grammar for codexe language 
Grammar as in (Declarataion, Expression, etc)

It checks for the tokens that should be in proper manner, otherwise throws error.

- AST(Abstract Syntax tree)
-Why AST?
An AST is a tree representation of Codexe source code(.cod).

It removes unnecessary syntax details and represents the meaningful structure of the program.

It makes the program easier to traverse and process during later compiler stages, such as semantic analysis and code generation.

It preserves the relationships between operators and operands, including operator precedence.

For ex:
int age = 5 + (9 * (4 - 1));

Tree goes like - 
                  int
          age            +
                    5         *
                          9        -
                                 4   1

The tree preserves the order of operations: first (4-1=3), then (9*3=27), and finally (5+27=32).

- Note:
. AST represents the expression
. Calculation of these expressions will be in further stages 



4. Semantic Analysis

- Symbol Table
Symbol table datastructure stores the information about identifiers.

- Why is there a need of symbol table?
. It tells us if the variable was created previously
. It stores new variables and their type

- Semantic Analysis
Semantic Analysis checks meaning of the program.
This makes sure that the code is not only syntactically correct but also meaningful.

- Note:
. It should not execute any part of the program while checking



5. Intermediate Representaion
Its a simple represntation of program that sits between source AST and machine code.

-Why IR
. Directly converting orignal AST to assemly is complicated
. IR coverts AST in simple instructions.
. Than that instructions are processed by assembly

- Three-Address Code(TAC)
TAC is used for intermediate representation in which each instruction contains at most three addresses(one destination address and two operand addresses).

For ex:
declare age : int = 20 + 0 + 9 + 8 + 7;

Output:
t1 = 20 + 0
t2 = t1 + 9
t3 = t2 + 8
t4 = t3 + 7
age = t4

 - Note:
Each arithmetic instruction has at most three addresses:
destination = operand1 operator operand2



6. Code Optimization
Code Optimization is the method that is handled during compilation phase. 
Its used to improve the generated Intermediate Representations to make program more efficient(saves time for runtime execution).
Optimization does not change program's orignal behavior

-Why used?
. Reduce unnecessary instructions
. Reduce execution time
. Reduce memory usage

-Techniques
1. Constant Folding
2. Constant Propogation

6.1 Constant Folding
It evaluates the expression containing constant values in operands.
For ex:
declare age : int = 20 + 0 + 9 + 8 + 7;

Output:
. IR Output(Before Optimization)
t1 = 20 + 0
t2 = t1 + 9
t3 = t2 + 8
t4 = t3 + 7
age = t4

. After optimization
t1 = 20
t2 = 29
t3 = 37
t4 = 44
age = 44

6.2 Constant Propogation
Replaces variable or temporary variables with constant values.

