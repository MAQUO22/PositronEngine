#pragma once

#include <vector>

#include "PositronEngineCore/ShaderProgram.hpp"
#include "PositronEngineCore/Texture2D.hpp"

#include <memory>

namespace PositronEngine
{
    struct LightReactionConfig
    {
        float ambient;
        float diffuse;
        float specular;
        float shininess;
    };

    class Material
    {
        public:
            Material() = delete;

            Material(std::vector<Texture2D>& textures,LightReactionConfig light_config);
            ~Material() = default;

            std::shared_ptr<ShaderProgram> getShaderProgram();
            std::vector<Texture2D>& getTexturesVector();
            LightReactionConfig& getLightConfig();
            void setLightConfig(LightReactionConfig& config);

        private:
            std::shared_ptr<ShaderProgram> _shader_program;
            std::vector<Texture2D> _textures;
            LightReactionConfig _light_config;

    };
}
