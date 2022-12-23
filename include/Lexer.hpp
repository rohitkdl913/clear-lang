#ifndef CLEAR_LEXER_H_
#define CLEAR_LEXER_H_
#include<vector>
#include<map>
#include "Token.hpp"

class Lexer
{
    std::map<std::string, TokenType> keywords;
    std::string lexingData;
    size_t currentPosition;
    size_t lineNumber;

public:
    Lexer(std::string &_lexingData) : lexingData(_lexingData)
    {
        currentPosition = 0;
        lineNumber = 1;
        keywords["let"] = TokenType::LET_KEYWORD;
        keywords["fun"] = TokenType::FUN_KEYWORD;
        keywords["if"] = TokenType::IF_KEYWORD;
        keywords["else"] = TokenType::ELSE_KEYWORD;
    }
    std::vector<Token> lex();
    void parseNumber(std::vector<Token> &tokens);
    void parseIdentifierWithCheck(std::vector<Token> &tokens);
};

std::vector<Token> Lexer::lex()
{
    std::vector<Token> _tokens;
    size_t dataLength = lexingData.length();
    for (; currentPosition < dataLength;)
    {
        if (isdigit(lexingData[currentPosition]))
        {
            parseNumber(_tokens);
            continue;
        }
        if (isalpha(lexingData[currentPosition]))
        {
            parseIdentifierWithCheck(_tokens);
            continue;
        }
        switch (lexingData[currentPosition])
        {
        case ':':
            _tokens.push_back(Token(TokenType::COLON, ":", currentPosition));
            break;
        case '=':
             if(lexingData[currentPosition + 1] == '='){
                _tokens.push_back(Token(TokenType::EQUAL_EQUAL, "==", currentPosition));
                currentPosition++;
             }else{
                _tokens.push_back(Token(TokenType::EQUAL, "=", currentPosition));
             }
            break;
         case '>':
           if (lexingData[currentPosition + 1] == '=')
           {
            _tokens.push_back(Token(TokenType::GREATER_THAN_OR_EQUAL, ">=", currentPosition));
            currentPosition++;
           }else{
            _tokens.push_back(Token(TokenType::GREATER_THAN, ">", currentPosition));
           }
            break;
        case '<':
           if (lexingData[currentPosition + 1] == '=')
           {
            _tokens.push_back(Token(TokenType::LESS_THAN_OR_EQUAL, "<=", currentPosition));
            currentPosition++;
           }else{
            _tokens.push_back(Token(TokenType::LESS_THAN, "<", currentPosition));
           }
            break;
        case '+':
            _tokens.push_back(Token(TokenType::PLUS, "+", currentPosition));
            break;
        case '-':
            _tokens.push_back(Token(TokenType::MINUS, "-", currentPosition));
            break;
        case '*':
            _tokens.push_back(Token(TokenType::STAR, "*", currentPosition));
            break;
        case '/':
            _tokens.push_back(Token(TokenType::FORWARD_SLASH, "/", currentPosition));
            break;
            case '(':
            _tokens.push_back(Token(TokenType::PAREN_OPEN, "(", currentPosition));
            break;
            case ')':
            _tokens.push_back(Token(TokenType::PAREN_CLOSE, ")", currentPosition));
            break;
            case ',':
            _tokens.push_back(Token(TokenType::COMMA, ",", currentPosition));
            break;
             case '{':
            _tokens.push_back(Token(TokenType::CURLY_OPEN, "{", currentPosition));
            break;
             case '}':
            _tokens.push_back(Token(TokenType::CURLY_CLOSE, "}", currentPosition));
            break;
        case ' ':
            break;
        case '\n':
            _tokens.push_back(Token(TokenType::END_OF_LINE, "\n", currentPosition));
            lineNumber = lineNumber + 1;
            break;
        default:
            break;
        }

        currentPosition++;
    }
    if (currentPosition == dataLength)
    {
        _tokens.push_back(Token(TokenType::END_OF_FILE, "End of file", currentPosition));
    }

    return _tokens;
}

void Lexer::parseNumber(std::vector<Token> &tokens)
{
    size_t start = currentPosition;
    while (isdigit(lexingData[currentPosition]) && lexingData[currentPosition] != ' ')
    {
        currentPosition++;
    }
    tokens.push_back(Token(TokenType::NUMBER, lexingData.substr(start, currentPosition - start), start));
}

void Lexer::parseIdentifierWithCheck(std::vector<Token> &tokens)
{
    size_t start = currentPosition;
    while (isalnum(lexingData[currentPosition]))
    {
        currentPosition++;
    }
     auto t1=keywords.count(lexingData.substr(start, currentPosition - start));

    if (keywords.count(lexingData.substr(start, currentPosition - start))==1)
    {
        tokens.push_back(Token(keywords[lexingData.substr(start, currentPosition - start)], lexingData.substr(start, currentPosition - start), start));
    }
    else
    {
        tokens.push_back(Token(TokenType::IDENTIFIER, lexingData.substr(start, currentPosition - start), start));
    }
}



#endif