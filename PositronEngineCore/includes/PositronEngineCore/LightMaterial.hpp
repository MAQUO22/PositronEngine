#pragma once

#include <vector>

#include "PositronEngineCore/ShaderProgram.hpp"
#include "PositronEngineCore/Texture2D.hpp"

namespace PositronEngine
{


    class LightMaterial
    {
        public:
            LightMaterial() = delete;

            LightMaterial(std::vector<Texture2D>& textures);
            ~LightMaterial();

            ShaderProgram* getShaderProgram();
            std::vector<Texture2D>& getTexturesVector();

        private:
            ShaderProgram* _shader_program = nullptr;
            std::vector<Texture2D> _textures;
    };
}
