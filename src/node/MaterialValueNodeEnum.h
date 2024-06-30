#pragma once

enum class MaterialValueNodeEnum
{
    AMBIENT,
    DIFFUSE,
    SPECULAR,
    ROUGHNESS,
    METALNESS,
    OPACITY,
    SHININESS,
    LINEAR,
    CONSTANT,
    QUADRATIC,
    NORMAL,
    TILE,

    // water
    TINT,
    TRANSPARENCY,
    FLOW,
    WAVE_STRENGTH,
    WAVE_SPEED,
    // volume
    DENSITY,

    REFLECTION,
    CUSTOM
};