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

            void draw(Camera& camera,std::vector<std::unique_ptr<LightObject>>& light_sources) override;
            void draw(std::shared_ptr<ShaderProgram>& shader_program, glm::mat4 space_matrix) override;
            void draw(std::shared_ptr<ShaderProgram>& shader_program, std::vector<glm::mat4> space_matrices) override;

            void setMaterial(const std::shared_ptr<Material>& material) override;

            std::shared_ptr<Material> getMaterial() override;
            std::unique_ptr<Mesh> getMesh();

            std::string getName() override;
            ObjectType getObjectType() override;

        private:
            std::string _name;
            ObjectType _type;
            std::unique_ptr<Mesh> _mesh;
            std::shared_ptr<Material> _material;

            //Material
            //Collision
            //и т.д.
    };
}

