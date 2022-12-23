
#include <fstream>
#include <sstream>
#include <memory>
#include "include/Lexer.hpp"
#include "include/Error.hpp"
#include "include/ASTNode.hpp"
#include "include/Parser.hpp"


void compile(std::string fileContent)
{
    Lexer _lexer(fileContent);
    std::vector<Token> tokens = _lexer.lex();
    auto parser=Parser(tokens);
    auto expr=parser.parse();
    //auto value=visitNode(expr.get());
    //std::cout<<"The result is "<<value<<std::endl;
   // expr->display(2);
}
std::string readFile(const char *fileName)
{
    std::cout << fileName << " is compiling" << std::endl;
    std::ifstream file(fileName);
    std::string data;
    std::ostringstream ss; // stream for reading whole file
    if (!file.is_open())
    {
        std::cout << "Error:Cannot open file " << fileName << std::endl;
    }
    ss << file.rdbuf(); // reading data
    data = ss.str();
    std::cout << data << std::endl;
    file.close();
    return data;
}

void displayHelp()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "       clearc <filename>" << std::endl;
    std::cout << "       clearc [options]  <filename>" << std::endl;
    std::cout << "       clearc --help" << std::endl;
}

void parseCommandLine(int argLength, char const *arg[])
{
    if (argLength == 1)
    {
        displayHelp();
        exit(1);
    }
    else if (argLength == 2)
    {
        std::string fileContent = readFile(arg[1]);
        compile(fileContent);
    }
    else
    {
        std::cout << "Other options are not implemented yet " << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    parseCommandLine(argc, argv);
    return 0;
}