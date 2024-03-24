#pragma once

#include "PositronEngineCore/GameObject.hpp"
#include "PositronEngineCore/LightObject.hpp"
#include "PositronEngineCore/Mesh.hpp"
#include "PositronEngineCore/Material.hpp"

namespace PositronEngine
{
    class CubePrimitive : public GameObject
    {
        public:
            CubePrimitive() = delete;
            CubePrimitive(std::string name);
            ~CubePrimitive();

            void draw(Camera& camera, LightObject& direction_light, LightObject& point_light) override;

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
