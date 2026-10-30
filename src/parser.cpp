
#include "../include/parser.h"
#include "../include/semantic.h"

SymbolTable st = SymbolTable();
Semantic sem = Semantic();

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

bool Parser::isComparisonOperator()
{
    if (current >= tokens.size())
        return false;

    return tokens[current].type == TokenType::GREATER_THAN ||
           tokens[current].type == TokenType::LESS_THAN ||
           tokens[current].type == TokenType::GREATER_EQUAL ||
           tokens[current].type == TokenType::LESS_EQUAL ||
           tokens[current].type == TokenType::EQUAL_EQUAL ||
           tokens[current].type == TokenType::NOT_EQUAL;
}

void Parser::printTokenData()
{
    if (current >= tokens.size())
        return;

    cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
    cout << "Value : " << tokens[current].value << endl;
    cout << "Line : " << tokens[current].line << endl;
}

void Parser::printNodeData(ASTNode *node)
{
    if (node == nullptr)
        return;

    cout << "Node - " << node->data << endl;

    printNodeData(node->left);
    printNodeData(node->right);

    for (ASTNode *statement : node->statements)
    {
        printNodeData(statement);
    }
}

void Parser::deleteNode(ASTNode *node)
{
    if (node == nullptr)
        return;

    deleteNode(node->left);
    deleteNode(node->right);

    for (ASTNode *statement : node->statements)
    {
        deleteNode(statement);
    }

    delete node;
}

ASTNode *Parser::ParseDeclaration()
{
    if (current >= tokens.size() ||
        tokens[current].type != TokenType::KEYWORD)
    {
        return nullptr;
    }

    string currType = tokens[current].value;

    ASTNode *declarationNode =
        new ASTNode(currType, ASTType::DECLARATION);

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::IDENTIFIER)
    {
        cout << "Error: Expected an identifier" << endl;
        deleteNode(declarationNode);
        return nullptr;
    }

    string currName = tokens[current].value;

    if (sem.reDeclaration(currName, st))
    {
        cout << "Error: Variable '" << currName
             << "' already declared" << endl;

        deleteNode(declarationNode);
        return nullptr;
    }

    ASTNode *identifierNode =
        new ASTNode(currName, ASTType::IDENTIFIER);

    declarationNode->left = identifierNode;

    advance();

    if (current < tokens.size() &&
        tokens[current].type == TokenType::SEMICOLON)
    {
        st.addSymbol(currName, currType);

        advance();

        cout << "Declaration AST:" << endl;
        printNodeData(declarationNode);

        return declarationNode;
    }

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::ASSIGNMENT ||
        tokens[current].value != "=")
    {
        cout << "Error: Expected '='" << endl;
        deleteNode(declarationNode);
        return nullptr;
    }

    advance();

    ASTNode *expressionNode = ParseExpression();

    if (expressionNode == nullptr)
    {
        cout << "Error: Expected an expression" << endl;
        deleteNode(declarationNode);
        return nullptr;
    }

    declarationNode->right = expressionNode;

    string expressionType = sem.getExpressionType(expressionNode, st);

    if (expressionType != currType)
    {
        cout << "Error: Type mismatch in declaration of '"
             << currName << "'" << endl;

        deleteNode(declarationNode);
        return nullptr;
    }

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::SEMICOLON)
    {
        cout << "Error: Expected ';'" << endl;
        deleteNode(declarationNode);
        return nullptr;
    }

    st.addSymbol(currName, currType);

    advance();

    cout << "Declaration AST:" << endl;
    printNodeData(declarationNode);

    return declarationNode;
}

ASTNode *Parser::ParseFactor()
{
    if (current >= tokens.size())
        return nullptr;

    if (tokens[current].type == TokenType::LEFT_PAREN)
    {
        advance();

        ASTNode *node = ParseExpression();

        if (node == nullptr)
            return nullptr;

        if (current >= tokens.size() ||
            tokens[current].type != TokenType::RIGHT_PAREN)
        {
            cout << "Error: Expected ')'" << endl;
            deleteNode(node);
            return nullptr;
        }

        advance();

        return node;
    }

    if (!isOperand())
        return nullptr;

    if (tokens[current].type == TokenType::IDENTIFIER)
    {
        string currName = tokens[current].value;

        if (!sem.checkVariable(currName, st))
            return nullptr;
    }

    ASTNode *operandNode = nullptr;

    if (tokens[current].type == TokenType::NUMBER)
    {
        operandNode =
            new ASTNode(tokens[current].value, ASTType::NUMBER);
    }
    else if (tokens[current].type == TokenType::STRING)
    {
        operandNode =
            new ASTNode(tokens[current].value, ASTType::STRING);
    }
    else if (tokens[current].type == TokenType::IDENTIFIER)
    {
        operandNode =
            new ASTNode(tokens[current].value, ASTType::IDENTIFIER);
    }

    advance();

    return operandNode;
}

ASTNode *Parser::ParseTerm()
{
    ASTNode *leftOperand = ParseFactor();

    if (leftOperand == nullptr)
        return nullptr;

    while (isMulDivOperator())
    {
        ASTNode *operatorNode = nullptr;

        if (tokens[current].type == TokenType::MUL_OPERATOR)
        {
            operatorNode =
                new ASTNode(tokens[current].value, ASTType::MUL_OPERATOR);
        }
        else
        {
            operatorNode =
                new ASTNode(tokens[current].value, ASTType::DIV_OPERATOR);
        }

        advance();

        ASTNode *rightOperand = ParseFactor();

        if (rightOperand == nullptr)
        {
            cout << "Error: Expected an expression" << endl;

            deleteNode(leftOperand);
            deleteNode(operatorNode);

            return nullptr;
        }

        operatorNode->left = leftOperand;
        operatorNode->right = rightOperand;

        leftOperand = operatorNode;
    }

    return leftOperand;
}

ASTNode *Parser::ParseExpression()
{
    ASTNode *leftOperand = ParseTerm();

    if (leftOperand == nullptr)
        return nullptr;

    while (isAddSubOperator())
    {
        ASTNode *operatorNode = nullptr;

        if (tokens[current].type == TokenType::ADD_OPERATOR)
        {
            operatorNode =
                new ASTNode(tokens[current].value, ASTType::ADD_OPERATOR);
        }
        else
        {
            operatorNode =
                new ASTNode(tokens[current].value, ASTType::SUB_OPERATOR);
        }

        advance();

        ASTNode *rightOperand = ParseTerm();

        if (rightOperand == nullptr)
        {
            cout << "Error: Expected an expression" << endl;

            deleteNode(leftOperand);
            deleteNode(operatorNode);

            return nullptr;
        }

        operatorNode->left = leftOperand;
        operatorNode->right = rightOperand;

        leftOperand = operatorNode;
    }

    return leftOperand;
}

ASTNode *Parser::ParseConditionals()
{
    if (current >= tokens.size() ||
        tokens[current].type != TokenType::IF)
    {
        return nullptr;
    }

    ASTNode *ifNode =
        new ASTNode(tokens[current].value, ASTType::IF);

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::LEFT_PAREN)
    {
        cout << "Error: Expected '('" << endl;
        deleteNode(ifNode);
        return nullptr;
    }

    advance();

    ASTNode *leftExpression = ParseExpression();

    if (leftExpression == nullptr)
    {
        cout << "Error: Expected left expression in condition" << endl;
        deleteNode(ifNode);
        return nullptr;
    }

    if (!isComparisonOperator())
    {
        cout << "Error: Expected a comparison operator" << endl;
        deleteNode(leftExpression);
        deleteNode(ifNode);
        return nullptr;
    }

    ASTNode *comparisonNode =
        new ASTNode(tokens[current].value, ASTType::COMPARISON_OPERATOR);

    ifNode->left = comparisonNode;
    comparisonNode->left = leftExpression;

    advance();

    ASTNode *rightExpression = ParseExpression();

    if (rightExpression == nullptr)
    {
        cout << "Error: Expected right expression in condition" << endl;
        deleteNode(ifNode);
        return nullptr;
    }

    comparisonNode->right = rightExpression;

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::RIGHT_PAREN)
    {
        cout << "Error: Expected ')'" << endl;
        deleteNode(ifNode);
        return nullptr;
    }

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::LEFT_BRACE)
    {
        cout << "Error: Expected '{' after if" << endl;
        deleteNode(ifNode);
        return nullptr;
    }

    advance();

    ASTNode *ifElseNode =
        new ASTNode("IF_ELSE_BLOCK", ASTType::BLOCK);
    ifNode->right = ifElseNode;

    ASTNode *ifBlock =
        new ASTNode("IF_BLOCK", ASTType::BLOCK);
    ifElseNode->left = ifBlock;

    while (current < tokens.size() &&
           tokens[current].type != TokenType::RIGHT_BRACE)
    {
        ASTNode *statementNode = ParseStatement();

        if (statementNode == nullptr)
        {
            deleteNode(ifNode);
            return nullptr;
        }

        ifBlock->statements.push_back(statementNode);
    }

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::RIGHT_BRACE)
    {
        cout << "Error: Expected '}' after if block" << endl;
        deleteNode(ifNode);
        return nullptr;
    }

    advance();

    if (current < tokens.size() && tokens[current].type == TokenType::ELSE)
    {
        advance();

        if (current >= tokens.size() ||
            tokens[current].type != TokenType::LEFT_BRACE)
        {
            cout << "Error: Expected '{' after else" << endl;
            deleteNode(ifNode);
            return nullptr;
        }

        advance();

        ASTNode *elseBlock = new ASTNode("ELSE_BLOCK", ASTType::BLOCK);
        ifElseNode->right = elseBlock;

        while (current < tokens.size() &&
               tokens[current].type != TokenType::RIGHT_BRACE)
        {
            ASTNode *statementNode = ParseStatement();

            if (statementNode == nullptr)
            {
                deleteNode(ifNode);
                return nullptr;
            }

            elseBlock->statements.push_back(statementNode);
        }

        if (current >= tokens.size() ||
            tokens[current].type != TokenType::RIGHT_BRACE)
        {
            cout << "Error: Expected '}' after else block" << endl;
            deleteNode(ifNode);
            return nullptr;
        }
        advance();

    }
    cout << "IF AST:" << endl;
    printNodeData(ifNode);

    return ifNode;
}

ASTNode *Parser::ParsePrint()
{
    if (current >= tokens.size() ||
        tokens[current].type != TokenType::KEYWORD ||
        tokens[current].value != "print")
    {
        return nullptr;
    }

    ASTNode *printNode =
        new ASTNode(tokens[current].value, ASTType::PRINT);

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::LEFT_PAREN)
    {
        cout << "Error: Expected '('" << endl;
        deleteNode(printNode);
        return nullptr;
    }

    advance();

    ASTNode *expressionNode = ParseExpression();

    if (expressionNode == nullptr)
    {
        cout << "Error: Expected an expression" << endl;
        deleteNode(printNode);
        return nullptr;
    }

    printNode->left = expressionNode;

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::RIGHT_PAREN)
    {
        cout << "Error: Expected ')'" << endl;
        deleteNode(printNode);
        return nullptr;
    }

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::SEMICOLON)
    {
        cout << "Error: Expected ';'" << endl;
        deleteNode(printNode);
        return nullptr;
    }

    advance();

    cout << "Print AST:" << endl;
    printNodeData(printNode);

    return printNode;
}

ASTNode *Parser::ParseAssignment()
{
    if (current >= tokens.size() ||
        tokens[current].type != TokenType::IDENTIFIER)
    {
        return nullptr;
    }

    string currName = tokens[current].value;

    if (!sem.checkVariable(currName, st))
        return nullptr;

    string variableType = st.getSymbolType(currName);

    ASTNode *identifierNode =
        new ASTNode(currName, ASTType::IDENTIFIER);

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::ASSIGNMENT ||
        tokens[current].value != "=")
    {
        cout << "Error: Expected '='" << endl;
        deleteNode(identifierNode);
        return nullptr;
    }

    ASTNode *assignmentNode =
        new ASTNode(tokens[current].value, ASTType::ASSIGNMENT);

    advance();

    ASTNode *expressionNode = ParseExpression();

    if (expressionNode == nullptr)
    {
        cout << "Error: Expected an expression" << endl;

        deleteNode(identifierNode);
        deleteNode(assignmentNode);

        return nullptr;
    }

    string expressionType = sem.getExpressionType(expressionNode, st);

    if (expressionType != variableType)
    {
        cout << "Error: Type mismatch in assignment to '"
             << currName << "'" << endl;

        deleteNode(identifierNode);
        deleteNode(expressionNode);
        deleteNode(assignmentNode);

        return nullptr;
    }

    assignmentNode->left = identifierNode;
    assignmentNode->right = expressionNode;

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::SEMICOLON)
    {
        cout << "Error: Expected ';'" << endl;
        deleteNode(assignmentNode);
        return nullptr;
    }

    advance();

    cout << "Assignment AST:" << endl;
    printNodeData(assignmentNode);

    return assignmentNode;
}

ASTNode *Parser::ParseStatement()
{
    if (current >= tokens.size())
        return nullptr;

    if (tokens[current].type == TokenType::KEYWORD &&
        (tokens[current].value == "int" ||
         tokens[current].value == "string"))
    {
        return ParseDeclaration();
    }
    else if (tokens[current].type == TokenType::KEYWORD &&
             tokens[current].value == "print")
    {
        return ParsePrint();
    }
    else if (tokens[current].type == TokenType::IDENTIFIER)
    {
        return ParseAssignment();
    }
    else if (tokens[current].type == TokenType::IF)
    {
        return ParseConditionals();
    }

    cout << "Error at line " << tokens[current].line
         << ": Unexpected token: " << tokens[current].value << endl;

    advance();

    return nullptr;
}

void Parser::ParseProgram()
{
    while (current < tokens.size())
    {
        ASTNode *statementNode = ParseStatement();

        if (statementNode == nullptr)
            return;

        deleteNode(statementNode);
    }
}
