#pragma once

#include "PositronEngineCore/Mesh.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/CubeMapTexture.hpp"
#include "PositronEngineCore/ShaderProgram.hpp"

#include <vector>

namespace PositronEngine
{
    class CubeMap
    {
        public:
            CubeMap() = delete;
            CubeMap(CubeMapTexture& cube_map_texture);
            ~CubeMap();

            CubeMapTexture* getCubeMapTexture();
            void draw(Camera& camera);

        private:
            CubeMapTexture* _cube_map_texture = nullptr;
            Mesh* _mesh = nullptr;
            ShaderProgram* _shader_program = nullptr;
    };
}
