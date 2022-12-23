#ifndef CLEAR_ERROR_H_
#define CLEAR_ERROR_H_
#include<vector>
typedef enum _errortype
{
    UNEXPECTED_TOKEN,
    INFO_ERROR
} ErrorType;
class Error
{  
public:
 ErrorType type;
   std::string infoString;
    Error(ErrorType _type,std::string _infoString):type(_type),infoString(_infoString){}
};

class ErrorManager
{
private:
 size_t numberOfError=0;   
public:
    
    void unexpected(std::string expected,std::string found);
    void errorLog(std::string infoString);
    void errorHelpInfo(std::string helpString);
};
void ErrorManager::unexpected(std::string expected,std::string found){
    numberOfError++;
    std::cout<<"Error : Expected "<<expected<<" token but found "<<found<<std::endl;
}
void ErrorManager::errorLog(std::string infoString)
{
    numberOfError++;
    std::cout<<"ERROR: "<<infoString<<std::endl;
}

void ErrorManager::errorHelpInfo(std::string helpString){
    std::cout<<helpString<<std::endl;
}

#endif