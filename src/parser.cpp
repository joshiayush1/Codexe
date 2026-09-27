#include "../include/parser.h"

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
    if (tokens[current].type == TokenType::NUMBER || tokens[current].type == TokenType::IDENTIFIER)
        return true;

    return false;
}

bool Parser::isAddSubOperator()
{
    if (tokens[current].type == TokenType::ADD_OPERATOR || tokens[current].type == TokenType::SUB_OPERATOR)
        return true;

    return false;
}

bool Parser::isMulDivOperator()
{
    if (tokens[current].type == TokenType::MUL_OPERATOR || tokens[current].type == TokenType::DIV_OPERATOR)
        return true;

    return false;
}

void Parser::printTokenData()
{
    cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
    cout << "Value : " << tokens[current].value << endl;
    cout << "Line : " << tokens[current].line << endl;
}

bool Parser::ParseDeclaration()
{
    if (tokens[current].type == TokenType::KEYWORD)
    {
        printTokenData();
        advance();
        if (tokens[current].type != TokenType::IDENTIFIER)
        {
            cout << "Expected an identifier" << endl;
            return false;
        }
    }

    if (tokens[current].type == TokenType::IDENTIFIER)
    {
        printTokenData();
        advance();
        if (tokens[current].type != TokenType::ASSIGNMENT)
        {
            cout << "Expected '='" << endl;
            return false;
        }
    }

    if (tokens[current].type == TokenType::ASSIGNMENT && tokens[current].value == "=")
    {
        printTokenData();
        advance();
        if (isOperand() || tokens[current].type == TokenType::LEFT_PAREN)
        {
            if (!ParseExpression())
            {
                return false;
            }
        }
        else
        {
            cout << "Expected a valid expression" << endl;
            return false;
        }
    }

    if (current >= tokens.size())
    {
        cout << "Expected ';' " << endl;
        return false;
    }
    if (tokens[current].type == TokenType::SEMICOLON && tokens[current].value == ";")
    {
        printTokenData();
        advance();
    }
    else
    {
        cout << "Expected ';' " << endl;
        return false;
    }
    return true;
}

// 13 is a expression
// 13 + 7 is a expression
// 13 + 5 + 2 is a expression
// 13 + + + 2 is not a expression
// 13 +  is a not a expression
// + 5 is not a expression


bool Parser::ParseFactor()
{
    if(tokens[current].type == TokenType::LEFT_PAREN){
        printTokenData();
        advance();

        if(!ParseExpression()){
            return false;
        }

        if(tokens[current].type != TokenType::RIGHT_PAREN){
            cout << "Expected ')'" << endl;
            return false;
        }
        printTokenData();
        advance();

            return true;
    }
    
    if (isOperand())
    {
        printTokenData();
        advance();
        return true;
    }


    return false;
}

bool Parser::ParseTerm()
{
    if (!ParseFactor())
    {
        return false;
    }
    while (isMulDivOperator())
    {
        printTokenData();
        advance();
        if (!ParseFactor())
        {
            cout << "Expected an expression" << endl;
            return false;
        }   
    }

    return true;
}

bool Parser::ParseExpression()
{
    if (!ParseTerm())
    {
        return false;
    }
    while (isAddSubOperator())
    {
        printTokenData();
        advance();
        if (!ParseTerm())
        {
            cout << "Expected an expression" << endl;
            return false;
        }
    }

    return true;
}

bool Parser::ParsePrint()
{
    if (tokens[current].type == TokenType::KEYWORD && tokens[current].value == "print")
    {
        printTokenData();
        advance();
        if (tokens[current].type == TokenType::LEFT_PAREN)  
        {
            printTokenData();
            advance();
        }
        else
        {
            cout << "Expected a '(" << endl;
            return false;
        }
        if (tokens[current].type == TokenType::IDENTIFIER)
        {
            printTokenData();
            advance();
        }
        else
        {
            cout << "Expected an expression" << endl;
            return false;
        }
        if (tokens[current].type == TokenType::RIGHT_PAREN)
        {
            printTokenData();
            advance();
        }
        else
        {
            cout << "Expected a ')' " << endl;
            return false;
        }
        if (tokens[current].type == TokenType::SEMICOLON)
        {
            printTokenData();
            advance();
        }
        else
        {
            cout << "Expected a ';' " << endl;
            return false;
        }
        return true;
    }
    return false;
}

bool Parser::ParseAssignment()
{
    if (tokens[current].type == TokenType::IDENTIFIER)
    {
        printTokenData();
        advance();

        if (tokens[current].type == TokenType::ASSIGNMENT)
        {
            printTokenData();
            advance();
        }
        else
        {
            cout << "Expected '='" << endl;
            return false;
        }
        if (isOperand() || tokens[current].type == TokenType::LEFT_PAREN)
        {
            if (!ParseExpression())
            {
                return false;
            }
        }
        else
        {
            cout << "Expected an expression" << endl;
            return false;
        }
        if (tokens[current].type == TokenType::SEMICOLON)
        {
            printTokenData();
            advance();
        }
        else
        {
            cout << "Expected ';'" << endl;
            return false;
        }
    }
    else
    {
        cout << "Expected an identifier" << endl;
        return false;
    }

    return true;
}

bool Parser::ParseStatement()
{
    if (tokens[current].type == TokenType::KEYWORD && tokens[current].value == "int")
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
        cout << "Unexpected token: " << tokens[current].value << endl;
        advance();
        return false;
    }
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