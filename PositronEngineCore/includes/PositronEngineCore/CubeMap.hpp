#pragma once

#include "PositronEngineCore/Mesh.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/CubeMapTexture.hpp"
#include "PositronEngineCore/ShaderProgram.hpp"

#include <vector>
#include <memory>

namespace PositronEngine
{
    class CubeMap
    {
        public:
            CubeMap() = delete;
            CubeMap(CubeMapTexture& cube_map_texture);
            ~CubeMap() = default;

            std::unique_ptr<CubeMapTexture> getCubeMapTexture();
            void draw(Camera& camera);

        private:
            std::unique_ptr<CubeMapTexture> _cube_map_texture;
            std::unique_ptr<Mesh> _mesh;
            std::shared_ptr<ShaderProgram> _shader_program;
    };
}
