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