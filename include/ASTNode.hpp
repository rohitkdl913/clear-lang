#ifndef CLEAR_ASTNODE_H_
#define CLEAR_ASTNODE_H_

#include <memory>
#include <string>
#include <iostream>
#include "Token.hpp"
//#include "CodeGen.hpp"

typedef enum
{
    NUMBER_AST,
    BINARY_EXPR_AST,
    UNARY_EXPR_AST,
    VARIABLE_EXPR_AST,
    VARIABLE_DECLARATION_AST,
    FUNCTION_AST,
    LOGICAL_EXPR_AST
} ASTType;

class BaseASTNode
{
public:
    virtual ~BaseASTNode() = default;
    virtual void display(int level) = 0;
    //virtual llvm::Value* codegen()=0;
};

class ExprASTNode : public BaseASTNode
{
public:
    virtual ~ExprASTNode() = default;
};

class NumExprASTNode : public ExprASTNode
{
public:
    double value;
    NumExprASTNode(double _value) : value(_value){}

    void display(int level) override
    {
        for (size_t i = 0; i < level; i++)
        {
            std::cout << " ";
        }
        std::cout << "Number AST:" << value << "\n";
    }
    // llvm::Value *codegen() override
    // {
    //     return llvm::ConstantFP::get(CodeGen::Context, llvm::APFloat(value));
    // }
};

class BinaryOpExprASTNode : public ExprASTNode
{
public:
    std::unique_ptr<ExprASTNode> left;
    std::unique_ptr<ExprASTNode> right;
    TokenType binaryOperator;
    BinaryOpExprASTNode(std::unique_ptr<ExprASTNode> _left,
                        std::unique_ptr<ExprASTNode> _right, TokenType _binaryOperator) : left(std::move(_left)), right(std::move(_right)), binaryOperator(_binaryOperator){}
    void display(int level) override
    {
        for (size_t i = 0; i < level; i++)
        {
            std::cout << " ";
        }
        std::cout << "Binary Expression:\n";
        left->display(level + 2);
        for (size_t i = 0; i < level + 2; i++)
        {
            std::cout << " ";
        }
        std::cout << "Operator:"
                  << "-\n";
        right->display(level + 2);
    }
    // llvm::Value *codegen() override
    // {
    //     return nullptr;
    // }
};

class LogicalExprASTNode : public ExprASTNode
{
public:
    std::unique_ptr<ExprASTNode> left;
    std::unique_ptr<ExprASTNode> right;
    TokenType logicalOperator;
    LogicalExprASTNode(std::unique_ptr<ExprASTNode> _left,
                       std::unique_ptr<ExprASTNode> _right, TokenType _logicalOperator) : left(std::move(_left)), right(std::move(_right)), logicalOperator(_logicalOperator){}
    void display(int level) override
    {
        for (size_t i = 0; i < level; i++)
        {
            std::cout << " ";
        }
        std::cout << "logical Expression:\n";
        left->display(level + 2);
        for (size_t i = 0; i < level + 2; i++)
        {
            std::cout << " ";
        }
        std::cout << "Operator:"
                  << "-\n";
        right->display(level + 2);
    }
    // llvm::Value *codegen() override
    // {
    //     return nullptr;
    // }
};

class UnaryOpExprASTNode : public ExprASTNode
{
private:
    std::unique_ptr<ExprASTNode> child;
    TokenType unaryOperator;

public:
    UnaryOpExprASTNode(std::unique_ptr<ExprASTNode> _child, TokenType _unaryOperator) : child(std::move(_child)), unaryOperator(_unaryOperator){}
    void display(int level) override
    {
        for (size_t i = 0; i < level; i++)
        {
            std::cout << " ";
        }
        std::cout << "Uanry Operator:"
                  << "-\n";
        child->display(level + 2);
    }
    // llvm::Value *codegen() override
    // {
    //     return nullptr;
    // }
};

class VariableASTNode : public ExprASTNode
{
private:
    std::string variableName;

public:
    VariableASTNode(std::string _variableName) : variableName(_variableName){}
    void display(int level) override
    {
        for (size_t i = 0; i < level; i++)
        {
            std::cout << " ";
        }
        std::cout << "Variable Name:" << variableName << "\n";
    }
    // llvm::Value *codegen() override
    // {
    //     llvm::Value *V = CodeGen::NamedValues[variableName];
    //     if (!V)
    //         std::cout << "Unknown variable name\n";
    //     return V;
    // }
};

class VariableSkeletonASTNode : public ExprASTNode
{
public:
    std::string variableName;
    int variableType;
    VariableSkeletonASTNode(std::string _variableName, int _variableType) : variableName(_variableName), variableType(_variableType) {}
    void display(int level) override
    {
        for (size_t i = 0; i < level; i++)
        {
            std::cout << " ";
        }
        std::cout << "Variable Skeleton:" << variableName << '\n';
        for (size_t i = 0; i < level + 2; i++)
        {
            std::cout << " ";
        }
        std::cout << "Type:" << variableType << '\n';
    }
    // llvm::Value *codegen() override
    // {
    //     return nullptr;
    // }
};

class VariableDeclareASTNode : public BaseASTNode
{
public:
    std::unique_ptr<VariableASTNode> variable;
    int variableType;
    std::unique_ptr<ExprASTNode> value;
    VariableDeclareASTNode(std::unique_ptr<VariableASTNode> _variable, int _variableType, std::unique_ptr<ExprASTNode> _value) : variable(std::move(_variable)), variableType(_variableType), value(std::move(_value)){}
    void display(int level) override
    {
        std::cout << "Variable Assignment:\n";
        variable->display(level + 2);
        value->display(level + 2);
    }
    // llvm::Value *codegen() override
    // {
    //     return nullptr;
    // }
};

class FunctionProtoTypeASTNode
{
public:
    std::string functionName;
    std::vector<std::unique_ptr<VariableSkeletonASTNode>> arguments;
    int returnType;
    FunctionProtoTypeASTNode(std::string _functionName, std::vector<std::unique_ptr<VariableSkeletonASTNode>> _arguments, int _returnType) : functionName(_functionName), arguments(std::move(_arguments)), returnType(_returnType) {}
    void display(int level)
    {
        std::cout << "Function ProtoType:" << functionName << '\n';
        for (auto &&element : arguments)
        {
            element->display(level + 2);
        }
    }
};

class BasicBlock
{
public:
    std::vector<std::unique_ptr<BaseASTNode>> instructions;
    void addInstructions(std::unique_ptr<BaseASTNode> instruction);
};
void BasicBlock::addInstructions(std::unique_ptr<BaseASTNode> instruction)
{
    instructions.push_back(std::move(instruction));
}

class FunctionASTNode : public BaseASTNode
{
public:
    std::unique_ptr<FunctionProtoTypeASTNode> protoType;
    std::unique_ptr<BasicBlock> block;
    FunctionASTNode(std::unique_ptr<FunctionProtoTypeASTNode> _protoType, std::unique_ptr<BasicBlock> _block) : protoType(std::move(_protoType)), block(std::move(_block)){}
    void display(int level) override
    {
        std::cout << "Function Defination:" << '\n';
        protoType->display(level + 2);
        // for (auto &&element : arguments)
        // {
        //     element->display(level + 2);
        // }
    }
    // llvm::Value* codegen()override{
    //     return nullptr;
    // }
};

#endif