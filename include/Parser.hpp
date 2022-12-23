#ifndef CLEAR_PARSER_H_
#define CLEAR_PARSER_H_
#include "ASTNode.hpp"
#include "Lexer.hpp"
#include "Error.hpp"
#include "Type.hpp"

class Parser
{
private:
    std::vector<Token> _tokens;
    Token *currentToken;
    size_t currentPosition = -1;
   
    ErrorManager errorManager;

public:
 Type *type;
    Parser(std::vector<Token> &tokens) : _tokens(tokens)
    {
        type = new Type();
        getNextToken();
    }
    std::unique_ptr<ExprASTNode> parseExpression();
    std::unique_ptr<ExprASTNode> parsePrimaryExpression();
    std::unique_ptr<BaseASTNode> parse();
    void getNextToken();
    std::unique_ptr<ExprASTNode> parseFactor();
    std::unique_ptr<ExprASTNode> parseTerm();
    std::unique_ptr<BaseASTNode> handleVariableDeclaration();
    std::unique_ptr<BaseASTNode> handleFunctionDefination();
    std::unique_ptr<FunctionProtoTypeASTNode> parseFunctionProtoType();
    std::unique_ptr<BasicBlock> parseBlock();
    std::unique_ptr<VariableSkeletonASTNode> parseVariableSkeleton();
    void parseIFStatement();
    void ignoreLineBreak()
    {
        while (currentToken->type == TokenType::END_OF_LINE)
            getNextToken();
    }
    void ignoreUptoNextLine()
    {
        while (currentToken->type != TokenType::END_OF_LINE && currentToken->type != TokenType::END_OF_FILE)
        {
            getNextToken();
        }
    }
    bool expect(TokenType expectedType, std::string expected)
    {
        if (currentToken->type == expectedType)
        {
            return true;
        }
        errorManager.unexpected(expected, currentToken->value);
        return false;
    }
    ~Parser();
};
Parser::~Parser()
{
    delete type;
   
}
std::unique_ptr<VariableSkeletonASTNode> Parser::parseVariableSkeleton()
{
    int variableTypeIndex;
    int isLegitVariableName;
    std::string variableName;
    if (currentToken->type == TokenType::IDENTIFIER)
    {
        isLegitVariableName = type->getDataTypeIndex(currentToken->value);
        if (isLegitVariableName != -1)
        {
            errorManager.unexpected("variable name", currentToken->value);
            errorManager.errorHelpInfo("Suggestion: variable name cannot be the existing data type.Use another variable name");
        }
        variableName = currentToken->value;
        getNextToken();
        if (!expect(TokenType::COLON, ":"))
        {
            return nullptr;
        }

        getNextToken();
        if (currentToken->type != TokenType::IDENTIFIER)
        {
            errorManager.unexpected("variable type", currentToken->value);
            return nullptr;
        }

        variableTypeIndex = type->getDataTypeIndex(currentToken->value);

        if (variableTypeIndex == -1)
        {
            errorManager.unexpected("variable type", currentToken->value);
            errorManager.errorHelpInfo("Suggestion: " + currentToken->value + "is not legit data type name");
        }
        if (variableTypeIndex == -1 || isLegitVariableName != -1)
        {
            return nullptr;
        }

        return std::make_unique<VariableSkeletonASTNode>(variableName, variableTypeIndex);
    }
    errorManager.unexpected("variable name", currentToken->value);
    return nullptr;
}
std::unique_ptr<BaseASTNode> Parser::handleVariableDeclaration()
{
    getNextToken();
    auto variableSkeleton = parseVariableSkeleton();
    if (variableSkeleton == nullptr)
    {
        ignoreUptoNextLine();
        return nullptr;
    }

    std::unique_ptr<ExprASTNode> expr = nullptr;
    getNextToken();
    if (!expect(TokenType::EQUAL, "=") && currentToken->type == TokenType::END_OF_LINE)
    {
        errorManager.errorHelpInfo("Suggestion: variable cannot be declared without initialized");
    }
    else
    {
        getNextToken();
        expr = parseTerm();
        if (!expect(TokenType::END_OF_LINE, "end of line") && currentToken->type != TokenType::END_OF_FILE)
        {
            ignoreUptoNextLine();
        }
    }
  
    return std::make_unique<VariableDeclareASTNode>(std::make_unique<VariableASTNode>(variableSkeleton->variableName), variableSkeleton->variableType, std::move(expr));
}

void Parser::parseIFStatement()
{
    getNextToken();
    parseTerm();
    switch (currentToken->type)
    {
    case TokenType::EQUAL_EQUAL:
        /* code */
        // break;
    case TokenType::LESS_THAN_OR_EQUAL:

    // break;
    case TokenType::GREATER_THAN_OR_EQUAL:
    // break;
    case TokenType::LESS_THAN:
    // break;
    case TokenType::GREATER_THAN:
        // break;
        getNextToken();
        break;
    case TokenType::EQUAL:
        errorManager.unexpected("logical expression", "assignment expression");
        getNextToken();
        break;
    default:
        errorManager.unexpected("logical operator", currentToken->value);
        getNextToken();
        break;
    }

    parseTerm();

    expect(TokenType::CURLY_OPEN, "{");
    getNextToken();
    parseBlock();
    expect(TokenType::CURLY_CLOSE, "}");
   getNextToken();
    ignoreLineBreak();
    if (currentToken->type == TokenType::ELSE_KEYWORD)
    {
        getNextToken();
        expect(TokenType::CURLY_OPEN, "{");
        getNextToken();
        parseBlock();
        expect(TokenType::CURLY_CLOSE, "}");
    }else{
        currentPosition-=2;
        getNextToken();
    }
}
std::unique_ptr<BasicBlock> Parser::parseBlock()
{
    auto block = std::make_unique<BasicBlock>();
    std::unique_ptr<BaseASTNode> instruction;
      getNextToken();
    while (currentToken->type != TokenType::CURLY_CLOSE && currentToken->type != TokenType::END_OF_FILE)
    {
      
        switch (currentToken->type)
        {
        case TokenType::LET_KEYWORD:
        {
            instruction = std::move(handleVariableDeclaration());
            if (instruction != nullptr)
            {
                block->addInstructions(std::move(instruction));
            }
        }
        break;
        case TokenType::IF_KEYWORD:
            parseIFStatement();
            
            break;
        case TokenType::END_OF_LINE:
        case TokenType::END_OF_FILE:
        case TokenType::CURLY_CLOSE:
            break;
        default:
            errorManager.errorLog("UnKnown token found" + currentToken->value);
            break;
        }
          getNextToken();
    }
    return block;
}
std::unique_ptr<BaseASTNode> Parser::handleFunctionDefination()
{
    auto functionProtoType = parseFunctionProtoType();
    std::unique_ptr<BasicBlock> block;
    getNextToken();
    ignoreLineBreak();
    if (expect(TokenType::CURLY_OPEN, "{"))
    {
        block = parseBlock();
        expect(TokenType::CURLY_CLOSE, "}");
    }
   
   return std::make_unique<FunctionASTNode>(std::move(functionProtoType),std::move(block));
}

std::unique_ptr<FunctionProtoTypeASTNode> Parser::parseFunctionProtoType()
{
    getNextToken();
    bool isThereError = false;
    std::vector<std::unique_ptr<VariableSkeletonASTNode>> arguments;
    std::unique_ptr<VariableSkeletonASTNode> argument;
    std::string functionName;
    bool islineignored = false;
    if (currentToken->type = TokenType::IDENTIFIER)
    {
        auto isLegitFunctionName = type->getDataTypeIndex(currentToken->value);
        if (isLegitFunctionName != -1)
        {
            errorManager.unexpected("function name", currentToken->value);
            errorManager.errorHelpInfo("Suggestion: function name cannot be the existing data type.Use another function name");
        }
        functionName = currentToken->value;
        getNextToken();
        expect(TokenType::PAREN_OPEN, "(");

        // check for blank argument

        getNextToken();

        if (currentToken->type != TokenType::PAREN_CLOSE)
        {
            currentPosition--;
            do
            {
                getNextToken();
                ignoreLineBreak();
                argument = parseVariableSkeleton();
                if (argument != nullptr)
                {
                    arguments.push_back(std::move(argument));
                    getNextToken();
                }
                else
                {
                    isThereError = true;
                }
            } while (currentToken->type == TokenType::COMMA /*&& currentToken->type!=TokenType::PAREN_CLOSE*/);
        }
        ignoreLineBreak();
        expect(TokenType::PAREN_CLOSE, ")");

        if (isThereError)
        {
            return nullptr;
        }

        return std::make_unique<FunctionProtoTypeASTNode>(functionName, std::move(arguments), 0);
    }
    return nullptr;
}

std::unique_ptr<BaseASTNode> Parser::parse()
{
    switch (currentToken->type)
    {
    case TokenType::LET_KEYWORD:
        return handleVariableDeclaration();
        break;
    case TokenType::FUN_KEYWORD:
        return handleFunctionDefination();
        break;
    default:
        break;
    }
    return std::make_unique<NumExprASTNode>(1212);
}

void Parser::getNextToken()
{
    currentPosition++;
    currentToken = &_tokens[currentPosition];
}

std::unique_ptr<ExprASTNode> Parser::parseExpression()
{
    return parseTerm();
}

std::unique_ptr<ExprASTNode> Parser::parseFactor()
{
    std::unique_ptr<ExprASTNode> left = parsePrimaryExpression();
    if (left == nullptr)
    {
        return nullptr;
    }
    std::unique_ptr<ExprASTNode> right;
    TokenType binaryOperator;
    while (currentToken->type == TokenType::STAR || currentToken->type == TokenType::FORWARD_SLASH)
    {
        binaryOperator = currentToken->type;
        getNextToken();
        right = parsePrimaryExpression();
        if (right == nullptr)
        {
            return left;
        }
        left = std::make_unique<BinaryOpExprASTNode>(std::move(left), std::move(right), binaryOperator);
    }
    return left;
}

std::unique_ptr<ExprASTNode> Parser::parseTerm()
{
    std::unique_ptr<ExprASTNode> left = parseFactor();
    if (left == nullptr)
    {
        return nullptr;
    }

    std::unique_ptr<ExprASTNode> right;
    TokenType binaryOperator;
    while (currentToken->type == TokenType::PLUS || currentToken->type == TokenType::MINUS)
    {
        binaryOperator = currentToken->type;
        getNextToken();
        right = parseFactor();
        if (right == nullptr)
        {
            return left;
        }
        left = std::make_unique<BinaryOpExprASTNode>(std::move(left), std::move(right), binaryOperator);
    }
    return left;
}

std::unique_ptr<ExprASTNode> Parser::parsePrimaryExpression()
{
    auto currToken = currentToken;
    switch (currentToken->type)
    {
    case TokenType::NUMBER:

        getNextToken();
        return std::make_unique<NumExprASTNode>(std::stod(currToken->value));
        break;
    case TokenType::IDENTIFIER:

        getNextToken();
        return std::make_unique<VariableASTNode>(currToken->value);
        // if (_tokens[currentPosition].type==TokenType::PAREN_OPEN)
        // {
        // we will handle function call here
        // }
    default:
        errorManager.unexpected("expression", currentToken->value);
        return nullptr;
    }
}

#endif