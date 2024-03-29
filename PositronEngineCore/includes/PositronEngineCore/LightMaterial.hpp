#pragma once

#include <vector>
#include <memory>

#include "PositronEngineCore/ShaderProgram.hpp"
#include "PositronEngineCore/Texture2D.hpp"

namespace PositronEngine
{


    class LightMaterial
    {
        public:
            LightMaterial() = delete;

            LightMaterial(std::vector<Texture2D>& textures);
            ~LightMaterial() = default;

            std::shared_ptr<ShaderProgram> getShaderProgram();
            std::vector<Texture2D>& getTexturesVector();

        private:
            std::shared_ptr<ShaderProgram> _shader_program;
            std::vector<Texture2D> _textures;
    };
}
