#include "../include/parser.h"
#include "../include/semantic.h"

SymbolTable st = SymbolTable();
Semantic sem = Semantic();
IR ir;

vector<ASTNode *> Parser::getAST()
{
    return programAST;
}

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
    // Declaration AST
    //                     int(declarationNode)

    // age(identifierNode)                      20(expressionNode)

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::KEYWORD ||
        tokens[current].value != "declare")
    {
        return nullptr;
    }

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::IDENTIFIER)
    {
        cout << "Error: Expected an identifier" << endl;
        return nullptr;
    }

    string currName = tokens[current].value;

    if (sem.reDeclaration(currName, st))
    {
        cout << "Error: Variable '" << currName
             << "' already declared" << endl;

        return nullptr;
    }

    ASTNode *identifierNode =
        new ASTNode(currName, ASTType::IDENTIFIER);

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::COLON)
    {
        cout << "Error: Expected a colon" << endl;
        deleteNode(identifierNode);
        return nullptr;
    }

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::KEYWORD ||
        (tokens[current].value != "int" && tokens[current].value != "string"))
    {

        cout << "Error: Expected a valid data type" << endl;
        deleteNode(identifierNode);
        return nullptr;
    }

    string currType = tokens[current].value;

    ASTNode *declarationNode =
        new ASTNode(currType, ASTType::DECLARATION);

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
    // Expression AST
    //                     +(operatorNode)

    // a(leftOperand)                       20(rightOperand)
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
    // Conditional AST
    //                                    if(ifNode)

    //             ==(comparisonNode)                         (if_else_block)

    //  age(leftExpression)  18(rightExpression)         (if_block)       (else_block)

    if (current >= tokens.size() ||
        (tokens[current].type != TokenType::KEYWORD &&
         tokens[current].value == "if"))
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
    cout << "Left Expression - ";
    printNodeData(leftExpression);
    cout << "Right Expression - ";
    printNodeData(rightExpression);
    cout << endl;

    comparisonNode->right = rightExpression;

    bool checkConditionlError = sem.checkConditionalErrors(leftExpression, rightExpression, comparisonNode, st);
    if (checkConditionlError == true)
    {
        deleteNode(ifNode);
        return nullptr;
    }

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

    st.enterScope();

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
    st.exitScope();

    if (current < tokens.size() && (tokens[current].type == TokenType::KEYWORD &&
                                    tokens[current].value == "else"))
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

        st.enterScope();

        ASTNode *elseBlock = new ASTNode("ELSE_BLOCK", ASTType::BLOCK);
        ifElseNode->right = elseBlock;

        while (current < tokens.size() &&
               tokens[current].type != TokenType::RIGHT_BRACE)
        {
            ASTNode *statementNode = ParseStatement();

            if (statementNode == nullptr)
            {
                st.exitScope();
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
        st.exitScope();
        advance();
    }
    cout << "IF AST:" << endl;
    printNodeData(ifNode);

    return ifNode;
}

ASTNode *Parser::ParseForLoop(){
    // ForLoop AST
    //                                          for(forNode)

    //               int(declarationNode)                                    within(conditionNode)

    //        it(iteratorNode) 0(iteratorValueNode)                 age(variableNode)    (for_block)
    

    if (current >= tokens.size() ||
        (tokens[current].type != TokenType::KEYWORD &&
         tokens[current].value == "for"))
    {
        return nullptr;
    }

    ASTNode *forNode =
        new ASTNode(tokens[current].value, ASTType::FOR);

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::LEFT_PAREN)
    {
        cout << "Error: Expected '('" << endl;
        deleteNode(forNode);
        return nullptr;
    }

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::IDENTIFIER)
    {
        cout << "Error: Expected an identifier" << endl;
        return nullptr;
    }

    string currName = tokens[current].value;

    if (sem.reDeclaration(currName, st))
    {
        cout << "Error: Variable '" << currName
             << "' already declared" << endl;

        return nullptr;
    }

    ASTNode *iteratorNode =
        new ASTNode(currName, ASTType::IDENTIFIER);

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::COLON)
    {
        cout << "Error: Expected a colon" << endl;
        deleteNode(iteratorNode);
        return nullptr;
    }

    advance();

    if (current >= tokens.size() ||
        tokens[current].type != TokenType::KEYWORD ||
        (tokens[current].value != "int" && tokens[current].value != "string"))
    {

        cout << "Error: Expected a valid data type" << endl;
        deleteNode(iteratorNode);
        return nullptr;
    }

    string currType = tokens[current].value;

    ASTNode *declarationNode =
        new ASTNode(currType, ASTType::DECLARATION);

    declarationNode->left = iteratorNode;

    advance();

    if (current >= tokens.size() || tokens[current].value != "=")
    {
        cout << "Error: Expected '='" << endl;
        deleteNode(iteratorNode);
        return nullptr;
    }

    ASTNode *assignmentNode =
        new ASTNode(tokens[current].value, ASTType::ASSIGNMENT);

    if (current >= tokens.size() || tokens[current].type != TokenType::NUMBER)
    {
        cout << "Error: Expected iterator value" << endl;
        deleteNode(iteratorNode);
        return nullptr;
    }

    ASTNode *valueNode =
        new ASTNode(tokens[current].value, ASTType::ASSIGNMENT);

    advance();

    ASTNode *withinNode =
        new ASTNode(tokens[current].value, ASTType::WITHIN);

    withinNode->left = iteratorNode;
    // comparisonNode->left = leftExpression;

    advance();

    ASTNode *rightExpression = ParseExpression();

    if (rightExpression == nullptr)
    {
        cout << "Error: Expected right expression in condition" << endl;
        deleteNode(ifNode);
        return nullptr;
    }
    cout << "Left Expression - ";
    printNodeData(leftExpression);
    cout << "Right Expression - ";
    printNodeData(rightExpression);
    cout << endl;

    comparisonNode->right = rightExpression;

    bool checkConditionlError = sem.checkConditionalErrors(leftExpression, rightExpression, comparisonNode, st);
    if (checkConditionlError == true)
    {
        deleteNode(ifNode);
        return nullptr;
    }

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

    st.enterScope();

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
    st.exitScope();

    if (current < tokens.size() && (tokens[current].type == TokenType::KEYWORD &&
                                    tokens[current].value == "else"))
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

        st.enterScope();

        ASTNode *elseBlock = new ASTNode("ELSE_BLOCK", ASTType::BLOCK);
        ifElseNode->right = elseBlock;

        while (current < tokens.size() &&
               tokens[current].type != TokenType::RIGHT_BRACE)
        {
            ASTNode *statementNode = ParseStatement();

            if (statementNode == nullptr)
            {
                st.exitScope();
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
        st.exitScope();
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
        tokens[current].value != "show")
    {
        return nullptr;
    }

    ASTNode *printNode =
        new ASTNode(tokens[current].value, ASTType::SHOW);

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
        tokens[current].value == "declare")
    {
        return ParseDeclaration();
    }
    else if (tokens[current].type == TokenType::KEYWORD &&
             tokens[current].value == "show")
    {
        return ParsePrint();
    }
    else if (tokens[current].type == TokenType::IDENTIFIER)
    {
        return ParseAssignment();
    }
    else if (tokens[current].type == TokenType::KEYWORD &&
             tokens[current].value == "if")
    {
        return ParseConditionals();
    }
    else if (tokens[current].type == TokenType::KEYWORD &&
             tokens[current].value == "for")
    {
        return ParseForLoop();
    }
    else if (tokens[current].type == TokenType::KEYWORD &&
             tokens[current].value == "while")
    {
        return ParseWhileLoop();
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

        programAST.push_back(statementNode);
    }
}
