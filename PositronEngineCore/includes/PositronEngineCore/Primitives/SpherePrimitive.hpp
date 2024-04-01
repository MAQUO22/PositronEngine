#pragma once

#include "PositronEngineCore/GameObject.hpp"
#include "PositronEngineCore/LightObject.hpp"
#include "PositronEngineCore/Mesh.hpp"
#include "PositronEngineCore/Material.hpp"
#include "PositronEngineCore/Sphere.h"

namespace PositronEngine
{
    class SpherePrimitive : public GameObject
    {
        public:
            SpherePrimitive() = delete;
            SpherePrimitive(std::string name);
            ~SpherePrimitive() = default;

            SpherePrimitive(const SpherePrimitive&);
            SpherePrimitive& operator=(const SpherePrimitive&);

            void draw(Camera& camera,std::vector<std::unique_ptr<LightObject>>& light_sources) override;
            void draw(std::shared_ptr<ShaderProgram>& shader_program,Camera& camera, glm::mat4 view) override;

            void setMaterial(const std::shared_ptr<Material>& material) override;

            std::shared_ptr<Material> getMaterial() override;
            std::shared_ptr<Mesh> getMesh();

            std::string getName() override;

        private:
            std::string _name;
            std::unique_ptr<Sphere> _sphere;
            std::shared_ptr<Mesh> _mesh;
            std::shared_ptr<Material> _material;

            //Material
            //Collision
            //и т.д.
    };
}

