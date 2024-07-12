#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace constants
{
    const std::string ASSET_EXT = ".asset";
    const std::string TEMP_FOLDER_NAME = "temp/";
    const std::string TEMP_ASSET_FOLDER_NAME = TEMP_FOLDER_NAME + "assets/";
    const std::string TEMP_MODEL_FODLER = "models";
    const std::string TEMP_TEX_FOLDER = "textures";

    const std::string ASSET_REAL_PATH = "../assets/";
    extern std::string ASSET_INSTALL_PATH;
    // geometry
    const short MAX_GEO_VARIANT = 10;
    const short MAX_CSM_NUMBER = 5;
    // config ini
    const std::string INI_PATH = "./config.ini";




    // TextureSlot
    const int IR_MAP = 24;
    const int PREFILTER_MAP = 25;
    const int BRDFLUT_MAP = 26;
    const int CSM   = 31;
    const int SHADOWCUBE = 30;
    const int SUN_CSM = 29;


    //Lights & Shadows
    const int MAX_POINT_LIGHTS = 10;
    const int MAX_DIR_LIGHTS = 10;
    const int MAX_CSM_LVLS = 5;

    // Terrain
    const int MAX_TERRAIN_RES_GPU = 20;
    const int MAX_TERRAIN_RES_CPU = 2048;
    const int HEIGHT_MAP = 23;  // texture slot
    const int NOISE_MAP = 22;   // texture slot

    //Atomsphere LUT
    /**
     *   glActiveTexture(GL_TEXTURE0 + transmittance_texture_unit);
  glBindTexture(GL_TEXTURE_2D, transmittance_texture_);
  int location = glGetUniformLocation(program, "transmittance_texture");
  glUniform1i(location, transmittance_texture_unit);

  glActiveTexture(GL_TEXTURE0 + scattering_texture_unit);
  glBindTexture(GL_TEXTURE_3D, scattering_texture_);
  location = glGetUniformLocation(program, "scattering_texture");
  glUniform1i(location,scattering_texture_unit);

  glActiveTexture(GL_TEXTURE0 + irradiance_texture_unit);
  glBindTexture(GL_TEXTURE_2D, irradiance_texture_);
  glUniform1i(glGetUniformLocation(program, "irradiance_texture"),
      irradiance_texture_unit);

    glActiveTexture(GL_TEXTURE0 + single_mie_scattering_texture_unit);
    glBindTexture(GL_TEXTURE_3D, optional_single_mie_scattering_texture_);
    glUniform1i(glGetUniformLocation(program, "single_mie_scattering_texture"),
        single_mie_scattering_texture_unit);
    */
   const int TRANSMITTANCE_TEXTURE = 18;
   const int SCATERING_TEXTURE = 19;
   const int IRRADIANCE_TEXTURE = 20;
   const int SINGLE_MIE_SCATTERING_TEXTURE = 21;


   // water
   const int FLOW_TEXTURE = 15;
   const int REFLECTION_TEXTURE = 16;
   const int REFRACTION_TEXTURE = 17;
   // frame cache

   const int FRAME_CACHE = 14;
   // bones
   const int MAX_BONE_PER_VERTEX = 4;
   const int MAX_BONE_NUM = 600;
};

class ConfigIniReader
{
public:
    static void  ReadConfigIni()
    {
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(constants::INI_PATH, pt);
        constants::ASSET_INSTALL_PATH = pt.get<std::string>("asset.ASSET_INSTALL_PATH");
    }
};
