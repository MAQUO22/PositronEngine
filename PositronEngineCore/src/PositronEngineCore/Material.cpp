#include "PositronEngineCore/Material.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{
    Material::Material(Texture2D* textures,LightReactionConfig light_config)
    {
        _shader_program = new ShaderProgram("default.vert", "default.frag", "default.geom");
        if(!_shader_program->isCompile())
            LOG_CRITICAL("Material not compiled!");

        //_textures = std::vector<Texture2D>(textures, textures + sizeof(&textures) / sizeof(Texture2D));
        size_t num_textures = sizeof(textures) / sizeof(Texture2D);

        // Копируем текстуры в вектор
        _textures = std::vector<Texture2D>(textures, textures + 3);
        _light_config = light_config;

    }

    ShaderProgram* Material::getShaderProgram()
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

    Material::~Material()
    {
        delete _shader_program;
    }
}
