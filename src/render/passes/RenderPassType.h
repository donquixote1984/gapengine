#pragma once
enum class RenderPassType
{

    // global level

    SHADOW, 
    REFLECTION, 
    REFRACTION,
    
    // MANITORY
    SCREEN,
    POSTPROCESS,

    // geometry level
    GEOMETRY,
    NOISE,
    CUSTOM,
    CANVAS,
    BICUBIC,
    CLOUD_NOISE,
    FRAME_CACHE,
    CLOUD,

    // debug
    FRAME_BUFFER_DEBUG
};