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
void Parser::printTokenData()
{
    cout << "TokenType : " << TokenTypeToString(tokens[current].type) << endl;
    cout << "Value : " << tokens[current].value << endl;
    cout << "Line : " << tokens[current].line << endl;
}

bool Parser::ParseDeclaration()
{
    if (tokens[current].type == TokenType::KEYWORD && tokens[current].value == "int")
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
        if (tokens[current].type == TokenType::NUMBER || tokens[current].type == TokenType::IDENTIFIER)
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

bool Parser::ParseExpression()
{
    int count = 1;
    if (tokens[current].type == TokenType::ADD_OPERATOR)
    {
        cout << "Invalid expression" << endl;
        return false;
    }
    while (current < tokens.size() && (tokens[current].type == TokenType::NUMBER ||
                                       tokens[current].type == TokenType::IDENTIFIER ||
                                       tokens[current].type == TokenType::ADD_OPERATOR ||
                                       tokens[current].type == TokenType::SUB_OPERATOR ||
                                       tokens[current].type == TokenType::MUL_OPERATOR ||
                                       tokens[current].type == TokenType::DIV_OPERATOR))
    {
        if (count % 2 == 1 && (tokens[current].type == TokenType::NUMBER || tokens[current].type == TokenType::IDENTIFIER)) // operand
        {
            printTokenData();
            advance();
            count++;
        }
        else if (count % 2 == 0 && (tokens[current].type == TokenType::ADD_OPERATOR ||
                                    tokens[current].type == TokenType::SUB_OPERATOR ||
                                    tokens[current].type == TokenType::MUL_OPERATOR ||
                                    tokens[current].type == TokenType::DIV_OPERATOR)) // operator
        {
            printTokenData();
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
    { // odd count means parser was expecting a operand(NUMBER or IDENTFIER) next.
        cout << "Invalid expression" << endl;
        return false;
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
        if (tokens[current].type == TokenType::NUMBER || tokens[current].type == TokenType::IDENTIFIER)
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