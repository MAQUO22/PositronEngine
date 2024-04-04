#pragma once

#include <json/json.hpp>
#include "PositronEngineCore/Mesh.hpp"
#include "PositronEngineCore/Material.hpp"
#include "PositronEngineCore/Camera.hpp"

#include "PositronEngineCore/ShaderProgram.hpp"

#include "PositronEngineCore/LightObject.hpp"
#include "PositronEngineCore/GameObject.hpp"

using JSON = nlohmann::json;

namespace PositronEngine
{
    class Model : public GameObject
    {
        public:
            Model(const char* file_name);

            void draw(Camera& camera, std::vector<std::unique_ptr<LightObject>>& light_sources) override;

            void draw(std::shared_ptr<ShaderProgram>& shader_program, glm::mat4 space_matrix) override;

            void setMaterial(const std::shared_ptr<Material>& material) override;
            std::shared_ptr<Material> getMaterial() override;

            std::vector<glm::mat4> matrices_meshes;
            glm::mat4 getModelMatrix(int index);

        private:
            const char* _file;
            std::vector<unsigned char> _data;
            JSON _json;

            std::shared_ptr<Material> _material;
            std::vector<std::shared_ptr<Mesh>> _meshes;

            std::vector<glm::vec3> translations_meshes;
            std::vector<glm::quat> rotations_meshes;
            std::vector<glm::vec3> scales_meshes;



            std::vector<unsigned char> getData();
            std::vector<float> getFloats(JSON accessor);
            std::vector<GLuint> getIndices(JSON accessor);

            std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions,
                                                 std::vector<glm::vec3> normals,
                                                 std::vector<glm::vec2> texture_uvs);

            void loadMesh(unsigned int index_mesh);
            void traverseNode(unsigned int next_node, glm::mat4 matrix = glm::mat4(1.0f));

            std::vector<glm::vec2> groupFloatsVec2(std::vector<float> float_vec);
            std::vector<glm::vec3> groupFloatsVec3(std::vector<float> float_vec);
            std::vector<glm::vec4> groupFloatsVec4(std::vector<float> float_vec);

    };
}
