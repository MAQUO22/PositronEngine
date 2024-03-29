#include "PositronEngineCore/LightMaterial.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{
    LightMaterial::LightMaterial(std::vector<Texture2D>& textures)
    {
        _shader_program = std::make_shared<ShaderProgram>("light.vert", "light.frag");
        if(!_shader_program->isCompile())
            LOG_CRITICAL("Material not compiled!");

        _textures = std::move(textures);

    }

    std::shared_ptr<ShaderProgram> LightMaterial::getShaderProgram()
    {
        return _shader_program;
    }

    std::vector<Texture2D>& LightMaterial::getTexturesVector()
    {
        return _textures;
    }
}
