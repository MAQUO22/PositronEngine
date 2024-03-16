#pragma once

#include "PositronEngineCore/GameObject.hpp"
#include "PositronEngineCore/Mesh.hpp"
#include "PositronEngineCore/Material.hpp"

namespace PositronEngine
{
    class SpherePrimitive : public GameObject
    {
        public:
            SpherePrimitive() = delete;
            SpherePrimitive(std::string name);
            ~SpherePrimitive();

            void draw(Camera& camera) override;

            void setMaterial(Material* material);
            Material* getMaterial();

            float light_position[3] = {0.0f, 0.0f, 0.0f};

        private:
            std::string _name;
            Mesh* _mesh = nullptr;
            Material* _material = nullptr;
            //Material
            //Collision
            //и т.д.
    };
}

