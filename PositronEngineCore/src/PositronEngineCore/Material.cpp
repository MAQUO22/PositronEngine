#include "PositronEngineCore/Material.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{
    Material::Material(std::vector<Texture2D>& textures,LightReactionConfig light_config)
    {
        _shader_program = std::make_shared<ShaderProgram>("default.vert", "default.frag", "default.geom");
        if(!_shader_program->isCompile())
            LOG_CRITICAL("Material not compiled!");

        _textures = std::move(textures);
        _light_config = light_config;

    }

    std::shared_ptr<ShaderProgram> Material::getShaderProgram()
    {
        return _shader_program;
    }

    std::vector<Texture2D>& Material::getTexturesVector()
    {
        return _textures;
    }

    LightReactionConfig& Material::getLightConfig()
    {
        return _light_config;
    }

    void Material::setLightConfig(LightReactionConfig& config)
    {
        _light_config = config;
    }
}
