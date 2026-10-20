Compiler using C++!

Codexe -> Lexical Analysis -> Parser



1. Define the codexe language

- We have to define rules for codexe
We define what are the rules of codexe language
Like int is a keyword
     age is a identifier



2. Lexer (Lexical Analyzer)
(KEYWORD, NUMBER, IDENTIFIER, OPERATOR, STRING) is our token type.

- We have to create tokens in order to share it to parser.

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
Grammar as in (Declrataion, Expression, etc)

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

- Why do we need symbol table?
. It tells us if the variable was created previously
. It stores new variables and their type

- Semantic Analysis
Semantic Analysis checks meaning of the program.
This makes sure that the code is not only syntactically correct but also meaningful.














Your original Codexe compiler roadmap
1. Lexical analysis
Identify keywords, identifiers, numbers, operators, and symbols.
Completed



2. Syntax analysis and AST
Parse statements and expressions into a tree.
Working



3. Semantic analysis
Check variable declarations, types, and valid variable usage.
Next stage



4. Symbol table
Track declared variables and their types.



5. Intermediate code generation
Convert AST expressions into three-address code (TAC).



6. Code optimization
Apply simple optimizations to intermediate code.



7. Code generation
Generate pseudo-assembly or assembly-like instructions in output.asm.