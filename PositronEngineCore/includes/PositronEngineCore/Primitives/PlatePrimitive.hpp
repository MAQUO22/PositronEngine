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
            ~PlatePrimitive() = default;

            PlatePrimitive(const PlatePrimitive&);
            PlatePrimitive& operator=(const PlatePrimitive&);

            void draw(Camera& camera, LightObject& direction_light, std::vector<std::unique_ptr<LightObject>>& point_lights) override;

            void setMaterial(const std::shared_ptr<Material>& material) override;

            std::shared_ptr<Material> getMaterial();
            std::unique_ptr<Mesh> getMesh();

            std::string getName() override;

        private:
            std::string _name;
            std::unique_ptr<Mesh> _mesh;
            std::shared_ptr<Material> _material;

            //Material
            //Collision
            //и т.д.
    };
}

