#include "PositronEngineCore/LightMaterial.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{
    LightMaterial::LightMaterial(std::vector<Texture2D>& textures)
    {
        _shader_program = new ShaderProgram("light.vert", "light.frag");
        if(!_shader_program->isCompile())
            LOG_CRITICAL("Material not compiled!");

        _textures = std::move(textures);

    }

    ShaderProgram* LightMaterial::getShaderProgram()
    {
        return _shader_program;
    }

    std::vector<Texture2D>& LightMaterial::getTexturesVector()
    {
        return _textures;
    }


    LightMaterial::~LightMaterial()
    {
        delete _shader_program;
    }
}
