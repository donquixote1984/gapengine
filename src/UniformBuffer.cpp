#include "UniformBuffer.h"



UniformBuffer::UniformBuffer()
{
    glGenBuffers(1, &m_UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferData(GL_UNIFORM_BUFFER, m_Size, NULL, GL_DYNAMIC_DRAW);
    //glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBO, 0, m_Size);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &m_UniformBufferAlignSize); // 256 
}

UniformBuffer::~UniformBuffer(){}

MatricesUniformBuffer * MatricesUniformBuffer::GetUniformBuffer()
{
    static MatricesUniformBuffer ub;
    return &ub;
}

LightUniformBuffer * LightUniformBuffer::GetUniformBuffer()
{
    static LightUniformBuffer ub;
    return &ub;
}
BoneUniformBuffer* BoneUniformBuffer::GetUniformBuffer()
{
    static BoneUniformBuffer ub;
    return &ub;
}


SettingsUniformBuffer * SettingsUniformBuffer::GetUniformBuffer()
{
    static SettingsUniformBuffer ub;
    return &ub;
}


void UniformBuffer::Feed(glm::mat4 &mat)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, m_CurrentOffset, sizeof(glm::mat4), glm::value_ptr(mat));
    m_CurrentOffset += sizeof(glm::mat4);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    if (m_CurrentOffset > MAX_OFFSET)
    {
        throw WrongAccumulationInRenderLoopException("uniform buffer pass max offset");
    }
}
void UniformBuffer::Feed(int num, int offset)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &num);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void UniformBuffer::Feed(glm::mat4 *mat, unsigned int offset)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), mat);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::Feed(glm::mat4 *matArrayHeader, unsigned int offset, unsigned int matArraySize)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4) * matArraySize, matArrayHeader);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void UniformBuffer::Bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
}
void UniformBuffer::BindShaderSlot(unsigned int shaderId, const char * channel, int slot, size_t size)
{
    unsigned int uboSlot = glGetUniformBlockIndex(shaderId, channel);
    glUniformBlockBinding(shaderId, uboSlot, slot);
    
    //glBindBufferRange(GL_UNIFORM_BUFFER, 2, m_UBO, 0, 150);
}
void UniformBuffer::BindShaderSlot(unsigned int shaderId, UniformBufferBindingSlot &slot)
{
    int uboSlot = glGetUniformBlockIndex(shaderId, slot.channel.c_str());
    if (uboSlot == -1)
    {
        return;
    }
    //glBindBufferBase(GL_UNIFORM_BUFFER, slot.slot, m_UBO);
    glUniformBlockBinding(shaderId, uboSlot, slot.slot);
    //slot.offset = m_NextBlockOffset;
    glBindBufferRange(GL_UNIFORM_BUFFER, slot.slot, m_UBO,  slot.offset, slot.preservedSize);
    //m_NextBlockOffset = m_UniformBufferAlignSize * (std::floor(slot.preservedSize / m_UniformBufferAlignSize) + 1);
}

void UniformBuffer::Reset()
{
    m_CurrentOffset = 0;
}

void LightUniformBuffer::Feed(int num, int offset)
{
    UniformBuffer::Feed(num, offset);
}
void LightUniformBuffer::Feed(ShaderPointLight *light, int count, int offset)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(ShaderPointLight) * count, light);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightUniformBuffer::Feed(ShaderDirLight *light, int count, int offset)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, 180* count, light);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightUniformBuffer::Feed(ShaderSunLight *light, int offset)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(ShaderSunLight), light);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void BoneUniformBuffer::Feed(glm::mat4* bones, int boneNum)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4) * boneNum, bones);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
