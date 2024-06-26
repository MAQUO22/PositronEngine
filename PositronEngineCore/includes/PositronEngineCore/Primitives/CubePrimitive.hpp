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
            ~CubePrimitive() = default;

            CubePrimitive(const CubePrimitive&);
            CubePrimitive& operator=(const CubePrimitive&);

            void draw(Camera& camera,std::vector<std::unique_ptr<LightObject>>& light_sources) override;
            void draw(std::shared_ptr<ShaderProgram>& shader_program, glm::mat4 space_matrix) override;
            void draw(std::shared_ptr<ShaderProgram>& shader_program, std::vector<glm::mat4> space_matrices) override;

            void setMaterial(const std::shared_ptr<Material>& material) override;

            std::shared_ptr<Material> getMaterial() override;
            std::shared_ptr<Mesh> getMesh();

            std::string getName() override;
            ObjectType getObjectType() override;

        private:
            std::string _name;
            ObjectType _type;
            std::shared_ptr<Mesh> _mesh;
            std::shared_ptr<Material> _material;

            //Collision
            //и т.д.
    };
}
