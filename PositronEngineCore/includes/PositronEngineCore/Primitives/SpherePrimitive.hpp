#pragma once

#include "PositronEngineCore/GameObject.hpp"
#include "PositronEngineCore/LightObject.hpp"
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

            void draw(Camera& camera, LightObject& light_object) override;

            void setMaterial(Material* material);
            Material* getMaterial();

        private:
            std::string _name;
            Mesh* _mesh = nullptr;
            Material* _material = nullptr;
            //Material
            //Collision
            //и т.д.
    };
}

