#include "UISettings.h"

namespace settings {
    UISettings::UISettings() {}
    UISettings::~UISettings() {}
    UISettings* UISettings::GetSettings()
    {
        static UISettings us;
        return &us;
    }
    void UISettings::ToggleGamma()
    {
        if (gamma != 0) {
            glEnable(GL_FRAMEBUFFER_SRGB);
        } else {
            glDisable(GL_FRAMEBUFFER_SRGB);
        }
    }

    void UISettings::FlushToShader()
    {
        ShaderSettings ss;
        ss.hasHDRIMap = envlight;
        ss.useShadowPCF = PCF;
        SettingsUniformBufferBindings::FeedSettings(&ss);
    }
}
