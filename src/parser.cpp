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

bool Parser::ParseDeclaration()
{
    if (tokens[current].type == TokenType::KEYWORD && tokens[current].value == "int")
    {
        cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
        cout << "Value : " << tokens[current].value << endl;
        cout << "Line : " << tokens[current].line << endl;
        advance();
        if (tokens[current].type != TokenType::IDENTIFIER)
        {
            cout << "Expected an identifier" << endl;
            return false;
        }
    }

    if (tokens[current].type == TokenType::IDENTIFIER)
    {
        cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
        cout << "Value : " << tokens[current].value << endl;
        cout << "Line : " << tokens[current].line << endl;
        advance();
        if (tokens[current].type != TokenType::ASSIGNMENT)
        {
            cout << "Expected '='" << endl;
            return false;
        }
    }

    if (tokens[current].type == TokenType::ASSIGNMENT && tokens[current].value == "=")
    {
        cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
        cout << "Value : " << tokens[current].value << endl;
        cout << "Line : " << tokens[current].line << endl;
        advance();
        if (tokens[current].type == TokenType::NUMBER)
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
        cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
        cout << "Value : " << tokens[current].value << endl;
        cout << "Line : " << tokens[current].line << endl;
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

bool Parser::ParseExpression()
{
    int count = 1;
    if (tokens[current].type == TokenType::ADD_OPERATOR)
    {
        cout << "Invalid expression" << endl;
        return 0;
    }
    while (current < tokens.size() &&
           (tokens[current].type == TokenType::NUMBER || tokens[current].type == TokenType::ADD_OPERATOR))
    {
        if (count % 2 == 1 && tokens[current].type == TokenType::NUMBER)
        {
            cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
            cout << "Value : " << tokens[current].value << endl;
            cout << "Line : " << tokens[current].line << endl;
            advance();
            count++;
        }
        else if (count % 2 == 0 && tokens[current].type == TokenType::ADD_OPERATOR)
        {
            cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
            cout << "Value : " << tokens[current].value << endl;
            cout << "Line : " << tokens[current].line << endl;
            advance();
            count++;
        }
        else
        {
            cout << "Invalid expression" << endl;
            count = -1;
            break;
        }
    }
    if (count == -1)
        return false;
    if (count % 2 == 1)
    { // odd count means parser was expecting a NUMBER next.
        cout << "Invalid expression" << endl;
        return false;
    }

    return true;
}

bool Parser::ParsePrint()
{
    if (tokens[current].type == TokenType::KEYWORD && tokens[current].value == "print")
    {
        cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
        cout << "Value : " << tokens[current].value << endl;
        cout << "Line : " << tokens[current].line << endl;
        advance();
        if (tokens[current].type == TokenType::LEFT_PAREN)
        {
            cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
            cout << "Value : " << tokens[current].value << endl;
            cout << "Line : " << tokens[current].line << endl;
            advance();
        }
        else
        {
            cout << "Expected a '(" << endl;
            return false;
        }
        if (tokens[current].type == TokenType::IDENTIFIER)
        {
            cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
            cout << "Value : " << tokens[current].value << endl;
            cout << "Line : " << tokens[current].line << endl;
            advance();
        }
        else
        {
            cout << "Expected an expression" << endl;
            return false;
        }
        if (tokens[current].type == TokenType::RIGHT_PAREN)
        {
            cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
            cout << "Value : " << tokens[current].value << endl;
            cout << "Line : " << tokens[current].line << endl;
            advance();
        }
        else
        {
            cout << "Expected a ')' " << endl;
            return false;
        }
        if (tokens[current].type == TokenType::SEMICOLON)
        {
            cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
            cout << "Value : " << tokens[current].value << endl;
            cout << "Line : " << tokens[current].line << endl;
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
    else
    {
        cout << "Unexpected token: " << tokens[current].value << endl;
        advance();
        return 0;
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