#ifndef _CLEAR_TYPE_H_
#define _CLEAR_TYPE_H_
#include <vector>
#include <string>
class Type
{
private:
    std::vector<std::string> _dataTypes;
    //std::vector<llvm::Type *> llvmTypes;

public:
    Type()
    {
        _dataTypes.push_back("i32");
        _dataTypes.push_back("i8");
        _dataTypes.push_back("i64");
        _dataTypes.push_back("f32");

        // llvmTypes.push_back(llvm::Type::getInt32Ty(CodeGen::Context));
        // llvmTypes.push_back(llvm::Type::getInt8Ty(CodeGen::Context));
        // llvmTypes.push_back(llvm::Type::getInt64Ty(CodeGen::Context));
        // llvmTypes.push_back(llvm::Type::getFloatTy(CodeGen::Context));
    }
    int getDataTypeIndex(std::string data);
};
int Type::getDataTypeIndex(std::string data)
{
    int index = -1;
    size_t dataTypeLength = _dataTypes.size();
    for (size_t i = 0; i < dataTypeLength; i++)
    {
        if (_dataTypes[i] == data)
        {
            index = i;
        }
    }
    return index;
}

#endif