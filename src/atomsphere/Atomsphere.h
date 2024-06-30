#pragma once
#include <math.h>
#include "DensityProfileLayer.h"
#include <vector>
#include <memory>
#include "../shaders/ShaderBatch.h"
#include "AtomsphereModel.h"
#include "../ScreenCanvas.h"
#include "../util/Util.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../textures/Texture.h"

class Light;
enum class Luminance {
    // Render the spectral radiance at kLambdaR, kLambdaG, kLambdaB.
    NONE,
    // Render the sRGB luminance, using an approximate (on the fly) conversion
    // from 3 spectral radiance values only (see section 14.3 in <a href=
    // "https://arxiv.org/pdf/1612.04336.pdf">A Qualitative and Quantitative
    //  Evaluation of 8 Clear Sky Models</a>).
    APPROXIMATE,
    // Render the sRGB luminance, precomputed from 15 spectral radiance values
    // (see section 4.4 in <a href=
    // "http://www.oskee.wz.cz/stranka/uploads/SCCG10ElekKmoch.pdf">Real-time
    //  Spectral Scattering in Large-scale Natural Participating Media</a>).
    PRECOMPUTED
};


using namespace atomsphere;
class Atomsphere
{
private:
    const int kLambdaMin = 360;
    const int kLambdaMax = 830;
      float m_vertices[20] = {
    -1, 1, 0, 0, 0,
    -1, -1, 0, 0, 1,
    1, -1, 0, 1, 1,
    1, 1, 0, 1, 0,
  };

  int indices[6] = {
      0, 1, 2, 2,3, 0
  };

    Texture * m_CloudNoise = nullptr;
    Texture * m_BlueNoise = nullptr;
    Texture * m_GrayNoise = nullptr;
    const double kSolarIrradiance[48] = {
        1.11776, 1.14259, 1.01249, 1.14716, 1.72765, 1.73054, 1.6887, 1.61253,
        1.91198, 2.03474, 2.02042, 2.02212, 1.93377, 1.95809, 1.91686, 1.8298,
        1.8685, 1.8931, 1.85149, 1.8504, 1.8341, 1.8345, 1.8147, 1.78158, 1.7533,
        1.6965, 1.68194, 1.64654, 1.6048, 1.52143, 1.55622, 1.5113, 1.474, 1.4482,
        1.41018, 1.36775, 1.34188, 1.31429, 1.28303, 1.26758, 1.2367, 1.2082,
        1.18737, 1.14683, 1.12362, 1.1058, 1.07124, 1.04992
    };
    // Values from http://www.iup.uni-bremen.de/gruppen/molspec/databases/
    // referencespectra/o3spectra2011/index.html for 233K, summed and averaged in
    // each bin (e.g. the value for 360nm is the average of the original values
    // for all wavelengths between 360 and 370nm). Values in m^2.
    const double kOzoneCrossSection[48] = {
        1.18e-27, 2.182e-28, 2.818e-28, 6.636e-28, 1.527e-27, 2.763e-27, 5.52e-27,
        8.451e-27, 1.582e-26, 2.316e-26, 3.669e-26, 4.924e-26, 7.752e-26, 9.016e-26,
        1.48e-25, 1.602e-25, 2.139e-25, 2.755e-25, 3.091e-25, 3.5e-25, 4.266e-25,
        4.672e-25, 4.398e-25, 4.701e-25, 5.019e-25, 4.305e-25, 3.74e-25, 3.215e-25,
        2.662e-25, 2.238e-25, 1.852e-25, 1.473e-25, 1.209e-25, 9.423e-26, 7.455e-26,
        6.566e-26, 5.105e-26, 4.15e-26, 4.228e-26, 3.237e-26, 2.451e-26, 2.801e-26,
        2.534e-26, 1.624e-26, 1.465e-26, 2.078e-26, 1.383e-26, 7.105e-27
    };

    const double kDobsonUnit = 2.687e20;
    // Maximum number density of ozone molecules, in m^-3 (computed so at to get
    // 300 Dobson units of ozone - for this we divide 300 DU by the integral of
    // the ozone density profile defined below, which is equal to 15km).
    const double kMaxOzoneNumberDensity = 300.0 * kDobsonUnit / 15000.0;
    // Wavelength independent solar irradiance "spectrum" (not physically
    // realistic, but was used in the original implementation).
    const double kConstantSolarIrradiance = 1.5;
    const double kBottomRadius = 6360000.0;
    const double kTopRadius = 6420000.0;
    const double kRayleigh = 1.24062e-6;
    const double kRayleighScaleHeight = 8000.0;
    const double kMieScaleHeight = 1200.0;
    const double kMieAngstromAlpha = 0.0;
    const double kMieAngstromBeta = 5.328e-3;
    const double kMieSingleScatteringAlbedo = 0.9;
    const double kMiePhaseFunctionG = 0.8;
    const double kGroundAlbedo = 0.1;
    const double max_sun_zenith_angle = 120.0 / 180.0 * M_PI;

    DensityProfileLayer rayleigh_layer;
    DensityProfileLayer mie_layer;
    std::vector<DensityProfileLayer> ozone_density;
    std::vector<double> wavelengths;
    std::vector<double> solar_irradiance;
    std::vector<double> rayleigh_scattering;
    std::vector<double> mie_scattering;
    std::vector<double> mie_extinction;
    std::vector<double> absorption_extinction;
    std::vector<double> ground_albedo;
    Shader * shader;

    //glm::vec3 sun_direction = {-0.28f,0.03f, -2.3f};
    float sun_holo_angle = 0;
    float exposure_ = 20.0;
    AtomsphereModel *model = nullptr;
    ScreenCanvas screenCanvas;
    float m_Frame = 0;
    Light* m_SunLight = nullptr;
    void InitWhitePoint();
public:
    Atomsphere();
    ~Atomsphere();
    void OnRender();
    glm::vec3 * GetSunDirection();
    Light* GetSunLight();
    const std::string GetShaderSource() const;
    glm::vec3 GetDepth();
    void Bind();
};