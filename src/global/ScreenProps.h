#pragma once

namespace Global
{

    struct Screen {
        float width;
        float height;
        float ratio;
        float dpr = 1.0f;
    };
    extern Screen screen;
    void SetScreenSize(float width, float height, float dpr);
};