#pragma once
#include "../FrameBuffer.h"
#include "../shaders/Shader.h"
#include "NoiseType.h"

class NoiseFrameBuffer: public FrameBuffer
{
private:
    float m_OffsetX = 0.0f;
    float m_OffsetY = 0.0f;

    int m_Frequency = 4;
    float m_Amplitude = 1.0f;
    int m_Octaves = 16;
public:
    NoiseFrameBuffer(NoiseType type, unsigned int res);
    ~NoiseFrameBuffer();
    void Render() override;
    void IncOffsetX(float inc);
    void IncOffsetY(float inc);
    void SetFrequency(int freq);
};