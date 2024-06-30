#include "ScreenProps.h"

namespace Global
{
    Screen screen;
}

void Global::SetScreenSize(float width, float height, float dpr)
{
    Global::screen.width = width;
    Global::screen.height = height;
    Global::screen.ratio = width / height;
    Global::screen.dpr = dpr;
}