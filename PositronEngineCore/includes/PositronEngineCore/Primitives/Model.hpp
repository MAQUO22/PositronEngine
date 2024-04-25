#pragma once

#include <glm/gtc/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "PositronEngineCore/Mesh.hpp"
#include "PositronEngineCore/Material.hpp"
#include "PositronEngineCore/Camera.hpp"

#include "PositronEngineCore/ShaderProgram.hpp"

#include "PositronEngineCore/LightObject.hpp"
#include "PositronEngineCore/GameObject.hpp"

namespace PositronEngine
{
    class Model : public GameObject
    {
        public:
            Model(const char* file_name, std::string name);

            void draw(Camera& camera, std::vector<std::unique_ptr<LightObject>>& light_sources) override;
            void draw(std::shared_ptr<ShaderProgram>& shader_program, glm::mat4 space_matrix) override;
            void draw(std::shared_ptr<ShaderProgram>& shader_program, std::vector<glm::mat4> space_matrices) override;

            void setMaterial(const std::shared_ptr<Material>& material) override;
            std::shared_ptr<Material> getMaterial() override;


        private:
            const char* _file;
            std::string _name;
            std::string _directory;

            std::shared_ptr<Material> _material;

            std::vector<Texture2D> textures_loaded;

            std::vector<std::shared_ptr<Mesh>> _meshes;

            void loadModel(std::string path);
            void loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string type_name);
            void processNode(aiNode *node, const aiScene* scene);
            std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene* scene);

    };
}
