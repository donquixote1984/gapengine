#pragma once
#include <vector>
#include "DensityProfileLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <functional>
#include <vector>
#include <array>
#include "AtomsphereConstants.h"
#include <glm/gtc/type_ptr.hpp>
#include "../util/Util.h"
namespace atomsphere {

class AtomsphereModel
{
public:

    AtomsphereModel(
    const std::vector<double>& wavelengths,
    const std::vector<double>& solar_irradiance,
    double sun_angular_radius,
    double bottom_radius,
    double top_radius,
    const std::vector<DensityProfileLayer>& rayleigh_density,
    const std::vector<double>& rayleigh_scattering,
    const std::vector<DensityProfileLayer>& mie_density,
    const std::vector<double>& mie_scattering,
    const std::vector<double>& mie_extinction,
    double mie_phase_function_g,
    const std::vector<DensityProfileLayer>& absorption_density,
    const std::vector<double>& absorption_extinction,
    const std::vector<double>& ground_albedo,
    double max_sun_zenith_angle,
    double length_unit_in_meters,
    unsigned int num_precomputed_wavelengths);
    void BindLut(GLuint transmittance_texture_unit, GLuint scattering_texture_unit, GLuint irradiance_texture_unit, GLuint single_mie_scattering_texture_unit);
    std::string shaderSource;
    ~AtomsphereModel();

    void Init(unsigned int num_scattering_orders = 4);

    GLuint shader() const { return atmosphere_shader_; }

    void SetProgramUniforms(
      GLuint program,
      GLuint transmittance_texture_unit,
      GLuint scattering_texture_unit,
      GLuint irradiance_texture_unit,
      GLuint optional_single_mie_scattering_texture_unit = 0) const;

    static void ConvertSpectrumToLinearSrgb(
      const std::vector<double>& wavelengths,
      const std::vector<double>& spectrum,
      float* r, float* g, float* b);


    static constexpr double kLambdaR = 680.0;
    static constexpr double kLambdaG = 550.0;
    static constexpr double kLambdaB = 440.0;

    unsigned int num_precomputed_wavelengths_;
    bool rgb_format_supported_;

    std::function<std::string(const glm::vec3&)> glsl_header_factory_;
    void Precompute(
    GLuint fbo,
    GLuint delta_irradiance_texture,
    GLuint delta_rayleigh_scattering_texture,
    GLuint delta_mie_scattering_texture,
    GLuint delta_scattering_density_texture,
    GLuint delta_multiple_scattering_texture,
    const glm::vec3& lambdas,
    const glm::mat3& luminance_from_radiance,
    bool blend,
    unsigned int num_scattering_orders);

    GLuint transmittance_texture_;
    GLuint scattering_texture_;
    GLuint optional_single_mie_scattering_texture_;
    GLuint irradiance_texture_;
    GLuint atmosphere_shader_;
    GLuint full_screen_quad_vao_;
    GLuint full_screen_quad_vbo_;
};
}