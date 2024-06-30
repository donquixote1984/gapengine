#include "UI.h"

struct NoiseSettings
{
    int freq = 4;
    float amp = 1.0f;
    int octaves = 1;
    float offset[2] = {0.0f, 0.0f};
};
class NoiseSettingUI: public UI
{
public:
    NoiseSettings settings;
    NoiseSettingUI(GLFWwindow *window);
    ~NoiseSettingUI();
    void RenderLoop() override;
};