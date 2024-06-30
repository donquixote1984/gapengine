#include "Atomsphere.h"
#include "../global/GlobalData.h"
#include "../global/ScreenProps.h"
#include "../global/Sky.h"
#include "../Camera.h"
#include "../lights/SunLight.h"
#include "../UniformBuffer.h"
using namespace atomsphere;

constexpr double kSunAngularRadius = 0.013235 / 2.0;
constexpr double kSunSolidAngle = M_PI* kSunAngularRadius * kSunAngularRadius;
constexpr double kLengthUnitInMeters = 1000.0;

Atomsphere::Atomsphere()
{
    rayleigh_layer = DensityProfileLayer(0.0, 1.0, -1.0 / kRayleighScaleHeight, 0.0, 0.0);
    mie_layer = DensityProfileLayer(0.0, 1.0, -1.0 / kMieScaleHeight, 0.0, 0.0);
    ozone_density.push_back(
    DensityProfileLayer(25000.0, 0.0, 0.0, 1.0 / 15000.0, -2.0 / 3.0));
    ozone_density.push_back(
      DensityProfileLayer(0.0, 0.0, 0.0, -1.0 / 15000.0, 8.0 / 3.0));

    sun_holo_angle = 0.1235 / 2.0;
   // sun_direction = {-0.28f,-0.092f, -2.3f};

    for (int l = kLambdaMin; l <= kLambdaMax; l += 10) {
        double lambda = static_cast<double>(l) * 1e-3;  // micro-meters
        double mie =
            kMieAngstromBeta / kMieScaleHeight * pow(lambda, -kMieAngstromAlpha);
        wavelengths.push_back(l);
        solar_irradiance.push_back(kSolarIrradiance[(l - kLambdaMin) / 10]);
        rayleigh_scattering.push_back(kRayleigh * pow(lambda, -4));
        mie_scattering.push_back(mie * kMieSingleScatteringAlbedo);
        mie_extinction.push_back(mie);
        absorption_extinction.push_back(kMaxOzoneNumberDensity * kOzoneCrossSection[(l - kLambdaMin) / 10]);
        ground_albedo.push_back(kGroundAlbedo);
    }

    model = new AtomsphereModel(
        wavelengths, 
        solar_irradiance, 
        kSunAngularRadius,
        kBottomRadius, 
        kTopRadius, 
        {rayleigh_layer}, 
        rayleigh_scattering,
        {mie_layer},
        mie_scattering, 
        mie_extinction, 
        kMiePhaseFunctionG,
        ozone_density, 
        absorption_extinction, 
        ground_albedo, 
        max_sun_zenith_angle,
        kLengthUnitInMeters, 
        3);

    model->Init();    
    m_CloudNoise = new Texture("res/textures/noise/bluenoise3.png", false, false);
    m_GrayNoise = new Texture("res/textures/noise/graynoiselg.png", false, false);
    Global::atomsphere = this;

    std::string skyShader = util::ReadFile("res/shaders/atomsphere/sky.shader");
    std::string modelSource = "#shader fragment\n" + model->shaderSource;
    shader = new ShaderBatch({skyShader, modelSource});

    shader->SetCurrentFolder("res/shaders/atomsphere/");
    shader->Ready();
    shader->Bind();
    model->SetProgramUniforms(shader->GetShaderId(), constants::TRANSMITTANCE_TEXTURE, constants::SCATERING_TEXTURE, constants::IRRADIANCE_TEXTURE, constants::SINGLE_MIE_SCATTERING_TEXTURE);

    shader->setUniform3f("earth_center", 0.0, (float) - kBottomRadius / kLengthUnitInMeters, 0);
    shader->setUniform2f("sun_size",  (float)tan(kSunAngularRadius), (float)cos(kSunAngularRadius));
    shader->setUniform1f("sun_holo_size", cos(sun_holo_angle));
    
    shader->BindToUniformBuffer(MatricesUniformBufferBindings::MatricesSlot);
    InitWhitePoint();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, (GLsizei)Global::screen.width, (GLsizei)Global::screen.height);
    m_SunLight = new SunLight();
    m_SunLight->SetDirection(glm::vec3(0.28f,-0.292f, 2.3f)); // for view test
    //m_SunLight->SetDirection(glm::normalize(glm::vec3(-5.0,-2.0f, -5.0f))); // for shadow test
}

void Atomsphere::Bind()
{
    model->BindLut(constants::TRANSMITTANCE_TEXTURE, constants::SCATERING_TEXTURE, constants::IRRADIANCE_TEXTURE, constants::SINGLE_MIE_SCATTERING_TEXTURE);
}

void Atomsphere::InitWhitePoint()
{
    float white_point_r = 1.0f;
    float white_point_g = 1.0f;
    float white_point_b = 1.0f;
    AtomsphereModel::ConvertSpectrumToLinearSrgb(wavelengths, solar_irradiance,
        &white_point_r, &white_point_g, &white_point_b);
    float white_point = (white_point_r + white_point_g + white_point_b) / 3.0f;
    white_point_r /= white_point;
    white_point_g /= white_point;
    white_point_b /= white_point;

    shader->setUniform3f("white_point", white_point_r, white_point_g, white_point_b);
}


void Atomsphere::OnRender()
{
    glViewport(0, 0, (GLsizei)Global::screen.width, (GLsizei)Global::screen.height);
    Camera *c = Global::camera;
    glm::vec3 pos = c->GetPosition();
    shader->Bind();
    shader->setUniform3f("camera", pos.x, pos.y, pos.z);
    shader->setUniform3f("earth_center", 0.0f, (float) - kBottomRadius / kLengthUnitInMeters, 0.0f);
    float t = glm::length(pos - glm::vec3(0, 0, (float) - kBottomRadius / kLengthUnitInMeters)) / (float)(kBottomRadius / kLengthUnitInMeters);
    shader->setUniform1f("exposure", exposure_);
    //shader->setUniform4m("model_from_view", c->GetView());
   // glm::vec3 sd = glm::normalize(glm::vec3(0, -1, 1));
    //shader->setUniform3f("sun_direction", sd.x, sd.y, sd.z);
    //glm::vec3 sun_direction = {-0.28f,0.03f, -2.3f};
    glm::vec3 normalizedSunDirection = -glm::normalize(m_SunLight->GetDirection());
    shader->setUniform3f("sun_direction", normalizedSunDirection.x, normalizedSunDirection.y, normalizedSunDirection.z);
 // memcpy(debugm, glm::value_ptr(lookat), sizeof(float) * 16);
    shader->setUniform1f("u_Time", static_cast<float>(glfwGetTime()));
    m_CloudNoise->Bind(2);
    shader->setUniform1i("u_CloudNoiseTexture", 2);
    m_GrayNoise->Bind(4);
    shader->setUniform1i("u_GrayNoiseTexture", 4);

    //glm::mat4 m = glm::perspective<float>(glm::radians(50.0), 1.77777779, 0.5f, 1000.0f);
   //shader->setUniform4m("view_from_clip", m);
   // screenCanvas.Draw();
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    screenCanvas.Draw();
    glDepthFunc(GL_LESS);
}


glm::vec3* Atomsphere::GetSunDirection()
{
    return &m_SunLight->GetDirection();
}
Atomsphere::~Atomsphere()
{
    delete model;
    model = NULL;
    delete m_SunLight;
    m_SunLight = NULL;
    delete m_CloudNoise;
    m_CloudNoise = NULL;
    delete m_GrayNoise;
    m_GrayNoise = NULL;
}
Light* Atomsphere::GetSunLight()
{
    return m_SunLight;
}

const std::string Atomsphere::GetShaderSource() const
{
    return "#shader fragment\n" + model->shaderSource;
}

glm::vec3 Atomsphere::GetDepth()
{
    Camera *c = Global::camera;
    return c->GetPosition() - glm::vec3(0, -kBottomRadius / kLengthUnitInMeters, 0);
}