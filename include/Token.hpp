#ifndef CLEAR_TOKEN_H_
#define CLEAR_TOKEN_H_
#include <iostream>
typedef enum _tokentype
{

    LET_KEYWORD,
    FUN_KEYWORD,
    IF_KEYWORD,
    ELSE_KEYWORD,



    EQUAL,
    PLUS,
    MINUS,
    STAR,
    FORWARD_SLASH,

    EQUAL_EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN_OR_EQUAL,
    NOT,

    IDENTIFIER,
    NUMBER,

   
    PAREN_OPEN,
    PAREN_CLOSE,
    CURLY_OPEN,
    CURLY_CLOSE,
    COLON,
    COMMA,


    END_OF_LINE,
    END_OF_FILE
} TokenType;

class Token
{
public:
    size_t start;
    TokenType type;
    std::string value;
    Token(TokenType _type, std::string _value, size_t _start) : type(_type), value(_value), start(_start) {}
    void display(std::string &data)
    {
        std::cout << "TokenType:" << type << std::endl;
        std::cout << "Value:" << value << std::endl;
    }
};
#endif