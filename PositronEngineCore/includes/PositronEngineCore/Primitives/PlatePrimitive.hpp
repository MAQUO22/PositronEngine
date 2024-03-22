#pragma once

#include "PositronEngineCore/GameObject.hpp"
#include "PositronEngineCore/LightObject.hpp"
#include "PositronEngineCore/Mesh.hpp"
#include "PositronEngineCore/Material.hpp"

namespace PositronEngine
{
    class PlatePrimitive : public GameObject
    {
        public:
            PlatePrimitive() = delete;
            PlatePrimitive(std::string name);
            ~PlatePrimitive();

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

