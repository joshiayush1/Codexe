#include "../include/parser.h"
#include "../include/semantic.h"

Parser::Parser()
{
    current = 0;
}

Token Parser::currentToken()
{
    return tokens[current];
}

void Parser::advance()
{
    current++;
}

bool Parser::isOperand()
{
    if (current >= tokens.size())
        return false;

    return tokens[current].type == TokenType::NUMBER ||
        tokens[current].type == TokenType::IDENTIFIER ||
        tokens[current].type == TokenType::STRING;
}

bool Parser::isAddSubOperator()
{
    if (current >= tokens.size())
        return false;

    return tokens[current].type == TokenType::ADD_OPERATOR || 
        tokens[current].type == TokenType::SUB_OPERATOR;
    
}

bool Parser::isMulDivOperator()
{
    if (current >= tokens.size())
        return false;
    return tokens[current].type == TokenType::MUL_OPERATOR || 
        tokens[current].type == TokenType::DIV_OPERATOR;
}

void Parser::printTokenData()
{
    cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
    cout << "Value : " << tokens[current].value << endl;
    cout << "Line : " << tokens[current].line << endl;
}

void Parser::printNodeData(ASTNode *node)
{
    if (node == nullptr)
    {
        return;
    }

    cout << "Node - " << node->data << endl;

    printNodeData(node->left);
    printNodeData(node->right);
}

void Parser::deleteNode(ASTNode *node)
{
    if (node == nullptr)
    {
        return;
    }

    deleteNode(node->left);
    deleteNode(node->right);

    delete (node);
}
SymbolTable st = SymbolTable();
Semantic sem = Semantic();

bool Parser::ParseDeclaration()
{
    if (current >= tokens.size() ||
        tokens[current].type != TokenType::KEYWORD)
    {
        return false;
    }
    ASTNode *declarationNode =
    new ASTNode(tokens[current].value, ASTType::DECLARATION);
    string currType = tokens[current].value;
    // cout<<currType<<endl;
    advance();
    
    if (current >= tokens.size() ||
    tokens[current].type != TokenType::IDENTIFIER)
    {
        cout << "Error: Expected an identifier" << endl;
        deleteNode(declarationNode);
        return false;
    }
    
    string currName = tokens[current].value;
    if(sem.reDeclaration(currName, st)){
       deleteNode(declarationNode);
       return false;
    }

    ASTNode *identifierNode =
    new ASTNode(tokens[current].value, ASTType::IDENTIFIER);
    declarationNode->left = identifierNode;
    
    advance();
    
    if (current < tokens.size() &&
    tokens[current].type == TokenType::SEMICOLON)
    {
        st.addSymbol(currName, currType);
        st.printSymbolType(currName);
        
        cout << "Declaration AST:" << endl;
        printNodeData(declarationNode);

        advance();
        deleteNode(declarationNode);
        return true;
    }

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::ASSIGNMENT ||
        tokens[current].value != "=")
    {
        cout << "Error: Expected '='" << endl;
        deleteNode(declarationNode);
        return false;
    }

    advance();

    if (current >= tokens.size())
    {
        cout << "Error: Expected an expression" << endl;
        deleteNode(declarationNode);
        return false;
    }

    ASTNode *expressionNode = ParseExpression();

    if (expressionNode == nullptr)
    {
        cout << "Error: Expected an expression" << endl;
        deleteNode(declarationNode);
        return false;
    }

    declarationNode->right = expressionNode;
    string expressionType = sem.getExpressionType(expressionNode, st);
    if (expressionType != currType)
    {
        cout << "Error: Type mismatch in declaration of '"
            << currName << "'" << endl;
        deleteNode(declarationNode);
        return false;
    }
    if (current >= tokens.size() ||
        tokens[current].type != TokenType::SEMICOLON)
    {
        cout << "Error: Expected ';'" << endl;
        deleteNode(declarationNode);
        return false;
    }
    st.addSymbol(currName, currType);
    st.printSymbolType(currName);
    
    cout << "Declaration AST:" << endl;
    printNodeData(declarationNode);

    advance();
    deleteNode(declarationNode);

    return true;
}

// 13 is a expression
// 13 + 7 is a expression
// 13 + 5 + 2 is a expression
// 13 + + + 2 is not a expression
// 13 +  is a not a expression
// + 5 is not a expression

ASTNode *Parser::ParseFactor()
{
    if (current >= tokens.size())
    {
        return nullptr;
    }
    if (tokens[current].type == TokenType::LEFT_PAREN)
    {
        // printTokenData();
        advance();

        ASTNode *node = ParseExpression();

        if (node == nullptr)
        {
            return nullptr;
        }

        if (current >= tokens.size() || tokens[current].type != TokenType::RIGHT_PAREN)
        {
            cout << "Error: Expected ')'" << endl;
            deleteNode(node);
            return nullptr;
        }

        advance();
        return node;
    }
    if (!isOperand())
    {
        return nullptr;
    }
    if(tokens[current].type == TokenType::IDENTIFIER){
        string currName = tokens[current].value;
        if(!sem.checkVariable(currName, st)){
            return nullptr;
        }
    }
    ASTNode *operandNode;
    if (tokens[current].type == TokenType::NUMBER)
    {
        operandNode = new ASTNode(tokens[current].value, ASTType::NUMBER);
    }
    else if (tokens[current].type == TokenType::STRING)
    {
        operandNode = new ASTNode(tokens[current].value, ASTType::STRING);
    }
    else if (tokens[current].type == TokenType::IDENTIFIER)
    {
        operandNode = new ASTNode(tokens[current].value, ASTType::IDENTIFIER);
    }
    // printTokenData();
    advance();

    return operandNode;
}

ASTNode *Parser::ParseTerm()
{
    ASTNode *leftOperand = ParseFactor();
    if (leftOperand == nullptr)
    {
        return nullptr;
    }

    while (isMulDivOperator())
    {
        ASTNode *operatorNode;
        if (tokens[current].type == TokenType::MUL_OPERATOR)
        {
            operatorNode = new ASTNode(tokens[current].value, ASTType::MUL_OPERATOR);
        }
        else if (tokens[current].type == TokenType::DIV_OPERATOR)
        {
            operatorNode = new ASTNode(tokens[current].value, ASTType::DIV_OPERATOR);
        }
        // printTokenData();
        advance();
        ASTNode *rightOperand = ParseFactor();
        if (rightOperand == nullptr)
        {
            cout << "Error: Expected an expression" << endl;
            deleteNode(leftOperand);
            delete operatorNode;
            return nullptr;
        }
        operatorNode->left = leftOperand;
        operatorNode->right = rightOperand;
        leftOperand = operatorNode;
    }

    return leftOperand; // point to entire subtree
}

ASTNode *Parser::ParseExpression()
{
    ASTNode *leftOperand = ParseTerm();
    if (leftOperand == nullptr)
    {
        return nullptr;
    }
    while (isAddSubOperator())
    {
        ASTNode *operatorNode;
        if (tokens[current].type == TokenType::ADD_OPERATOR)
        {
            operatorNode = new ASTNode(tokens[current].value, ASTType::ADD_OPERATOR);
        }
        else if (tokens[current].type == TokenType::SUB_OPERATOR)
        {
            operatorNode = new ASTNode(tokens[current].value, ASTType::SUB_OPERATOR);
        }
        // printTokenData();
        advance();

        ASTNode *rightOperand = ParseTerm();
        if (rightOperand == nullptr)
        {
            deleteNode(leftOperand);
            delete operatorNode;
            return nullptr;
        }
        operatorNode->left = leftOperand;
        operatorNode->right = rightOperand;
        leftOperand = operatorNode;
    }

    return leftOperand;
}

bool Parser::ParsePrint()
{
    ASTNode *printNode;
    if (tokens[current].type == TokenType::KEYWORD && tokens[current].value == "print")
    {
        printNode = new ASTNode(tokens[current].value, ASTType::PRINT);
        // printTokenData();
        advance();
        if (current < tokens.size() && tokens[current].type == TokenType::LEFT_PAREN)
        {
            // printTokenData();
            advance();
        }
        else
        {
            cout << "Error: Expected '(" << endl;
            delete (printNode);
            return false;
        }
        ASTNode *expressionNode = ParseExpression();

        if (expressionNode == nullptr)
        {
            cout << "Error: Expected an expression" << endl;
            deleteNode(printNode);
            return false;
        }

        printNode->left = expressionNode;
        if (current < tokens.size() && tokens[current].type == TokenType::RIGHT_PAREN)
        {
            // printTokenData();
            advance();
        }
        else
        {
            cout << "Error: Expected ')' " << endl;
            deleteNode(printNode);
            return false;
        }
        if (current < tokens.size() && tokens[current].type == TokenType::SEMICOLON)
        {
            // printTokenData();
            cout << "Print AST" << endl;
            printNodeData(printNode);
            if (current < tokens.size())
            {
                advance();
            }
            deleteNode(printNode);
        }
        else
        {
            cout << "Error: Expected a ';' " << endl;
            deleteNode(printNode);
            return false;
        }
        return true;
    }
    return false;
}

bool Parser::ParseAssignment()
{
    ASTNode *identifierNode;
    ASTNode *assignmentNode;
    if (tokens[current].type == TokenType::IDENTIFIER)
    {
        string currName = tokens[current].value;
        if(!sem.checkVariable(currName, st)){
            return false;
        }
        string variableType = st.getSymbolType(currName);

        identifierNode = new ASTNode(tokens[current].value, ASTType::IDENTIFIER);
        
        advance();
        
        if (current < tokens.size() && tokens[current].type == TokenType::ASSIGNMENT)
        {
            assignmentNode = new ASTNode(tokens[current].value, ASTType::ASSIGNMENT);
            advance();
        }
        else
        {
            delete(identifierNode);
            cout << "Error: Expected '='" << endl;
            return false;
        }
        if (current >= tokens.size() ||
        (!isOperand() && tokens[current].type != TokenType::LEFT_PAREN))
        {
            cout << "Error: Expected an expression" << endl;
            delete(identifierNode);
            delete(assignmentNode);
            return false;
        }
        ASTNode *expressionNode = ParseExpression();
        if(expressionNode == nullptr)
        {
            cout << "Error: Expected an expression" << endl;
            deleteNode(identifierNode);
            deleteNode(assignmentNode);
            return false;
        }
        string expressionType = sem.getExpressionType(expressionNode, st);
        if(expressionType != variableType){
            cout << "Type mismatch in assigment to '" << currName << "'" << endl;
            delete(identifierNode);
            delete(expressionNode);
            delete(assignmentNode);
            return false;
        }
        assignmentNode->left = identifierNode;
        assignmentNode->right = expressionNode;

        if (current < tokens.size() && tokens[current].type == TokenType::SEMICOLON)
        {
            // printTokenData();
            advance();
            cout << "Assigment AST" << endl;
            printNodeData(assignmentNode);
            deleteNode(assignmentNode);
        }
        else
        {
            cout << "Error: Expected ';'" << endl;
            deleteNode(assignmentNode);
            return false;
        }
    }
    else
    {
        cout << "Error: Expected an identifier" << endl;
        return false;
    }

    return true;
}

bool Parser::ParseStatement()
{
    if (current >= tokens.size())
    {
        return false;
    }
    if (tokens[current].type == TokenType::KEYWORD && 
        (tokens[current].value == "int" || tokens[current].value == "string"))
    {
        return ParseDeclaration();
    }
    else if (tokens[current].type == TokenType::KEYWORD && tokens[current].value == "print")
    {
        return ParsePrint();
    }
    else if (tokens[current].type == TokenType::IDENTIFIER)
    {
        return ParseAssignment();
    }
    else
    {
        if(current < tokens.size()){
            cout << "Error at line " << tokens[current].line << ": Unexpected token: " << tokens[current].value << endl;
            advance();
            return false;
        }
    }
    return false;
}

void Parser::ParseProgram()
{
    while (current < tokens.size())
    {
        if (!ParseStatement())
        {
            return;
        }
    }
}