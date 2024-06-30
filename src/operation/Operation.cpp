#include "Operation.h"
#include "../exceptions/InvalidTypeException.h"
#include "../textures/TextureType.h"

AssignTextureOperation::AssignTextureOperation(std::unordered_map<std::string, std::string> opParams)
{
    std::string textureTypeStr = opParams["type"];
    std::string textureFilePathStr = opParams["file"];
    m_TextureType = util::GetTextureType(textureTypeStr);
    m_TextureFilePath = textureFilePathStr;

}
void AssignTextureOperation::assignTarget(void * g)
{
    m_Target = g;
}

void AssignTextureOperation::operate()
{
    if (m_Target == nullptr)
    {
        throw InvalidTypeException("can not make operation on null ptr");
    }

    if (m_TextureType!= TextureType::NO_TEXTURE)
    {
        //static_cast<Geometry *>(m_Target)->AttachTexture(m_TextureFilePath, m_TextureType);
    }
}
