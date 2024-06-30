#pragma once
#include <string>
#include <boost/algorithm/string.hpp>
enum class TextureType
{
    NO_TEXTURE,
    DIFFUSE_TEXTURE, 
    SPECULAR_TEXTURE,
    NORMAL_TEXTURE,
    ROUGHNESS_TEXTURE,
    AO_TEXTURE,
    OPACITY_TEXTURE,
    METALNESS_TEXTURE,
};