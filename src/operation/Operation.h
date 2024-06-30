#pragma once
#include "../geometry/Geometry.h"
#include <unordered_map>
#include "../textures/TextureType.h"
#include "../exceptions/UnsupportedOperationException.h"
#include "fmt/format.h"
#include "../util/JsonUtil.h"
#include <unordered_map>
#include <any>

class Operation // Command interface
{
protected:
    void * m_Target = nullptr;
public:
    inline static const std::string OP_NAME = "OP_NAME";
    inline static const std::string OP_TYPE = "OP_TYPE";
    inline static const std::string OP_PARAM = "OP_PARAM";
    virtual void assignTarget(void *){};
    virtual void operate(){};
};


class OperationFactory
{
public:
    virtual ~OperationFactory() {};
    virtual Operation * GetOperation() = 0;
};


class AssignTextureOperation : public Operation
{
private:
    TextureType m_TextureType;
    std::string m_TextureFilePath;
public:
    AssignTextureOperation(std::unordered_map<std::string, std::string> opParams);

    void assignTarget(void *) override;
    void operate() override;
};


class TextureOperationFactory: public OperationFactory
{
private:
    std::unordered_map<std::string, std::any> operationParams;
public:
    TextureOperationFactory(std::unordered_map<std::string, std::any> operationParams):operationParams(operationParams) {}
    ~TextureOperationFactory() {}
    Operation * GetOperation() override
    {
        if (operationParams.find(Operation::OP_TYPE) == operationParams.end())
        {
            throw UnsupportedOperationException("unsupport texture operation without OP_TYPE");
        }

        std::string opType = std::any_cast<std::string>(operationParams[Operation::OP_TYPE]);
        std::unordered_map<std::string, std::string> opParam = std::any_cast<std::unordered_map<std::string, std::string>>(operationParams[Operation::OP_PARAM]);
        if (opType == "ASSIGN")
        {
            return new AssignTextureOperation(opParam);
        }
        
        std::string exp = fmt::format("unsupported texture operation with OP_TYPE: ", opType);
        throw UnsupportedOperationException(exp.c_str());
    }
};

class OperationFactoryBuilder {
public:
    static OperationFactory * GetOperationFactory(std::unordered_map<std::string, std::any> operationParams)
    {
        if (operationParams.find(Operation::OP_NAME) == operationParams.end())
        {
            throw UnsupportedOperationException("unsupport operation without OP_NAME");
        }

        std::string opName = std::any_cast<std::string>(operationParams[Operation::OP_NAME]);
        if (opName == "TEX")
        {
            return new TextureOperationFactory(operationParams);
        }

        std::string exp = fmt::format("unsupport operation: {}", opName);
        throw UnsupportedOperationException(exp.c_str());
    }
};