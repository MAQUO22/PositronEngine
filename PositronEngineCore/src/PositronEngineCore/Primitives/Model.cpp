#include "PositronEngineCore/Primitives/Model.hpp"
#include "PositronEngineCore/Log.hpp"

#include <fstream>
#include <PositronEngineCore/RenderOpenGL.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>


namespace PositronEngine
{
    struct MapsForMesh
    {
        std::vector<std::string> diffuse;
        std::vector<std::string> normal;
        std::vector<std::string> roughness;
    };

    const std::string PATH_TO_MODEL = "../../ResourceFiles/Models/";

    std::string getFileContext(const char* file_name)
    {
        LOG_INFORMATION("file_path -> {0}", file_name);
        std::ifstream in(file_name, std::ios::binary);
        if(in)
        {
            std::string file;
            in.seekg(0, std::ios::end);
            file.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&file[0], file.size());
            in.close();
            return(file);
        }
        LOG_CRITICAL("FILE CONTEXT IS NOT READ FROM THE FILE !");
        throw(errno);
    }

    std::string getDirectoryFromPath(const std::string& path)
    {
        std::size_t lastSlashPos = path.find_last_of('/');
        std::size_t lastBackslashPos = path.find_last_of('\\');

        if (lastSlashPos == std::string::npos && lastBackslashPos == std::string::npos) {
            // Нет символов '/', '\' в пути, возвращаем пустую строку
            return "";
        }

        std::size_t lastSeparatorPos = (lastSlashPos != std::string::npos) ? lastSlashPos : lastBackslashPos;

        return path.substr(0, lastSeparatorPos);
    }

    void Model::loadModel(std::string path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_CRITICAL("MODEL IMPORT ERROR: {0}", importer.GetErrorString());
            return;
        }

        _is_loaded = true;
        _directory = getDirectoryFromPath(path);
        processNode(scene->mRootNode, scene);
        LOG_INFORMATION("DIRECTORY -> {0}", _directory);

    }

    Model::Model(const char* file_name, std::string name)
    {
        _name = name;
        _type = ObjectType::model;
        loadModel(file_name);
        _material = std::make_shared<Material>();
    }

    void Model::processNode(aiNode *node, const aiScene* scene)
    {
        for(size_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            _meshes.push_back(processMesh(mesh, scene));
        }

        for(size_t i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    std::shared_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        for(size_t i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;

            if(mesh->mTextureCoords[0])
            {
                glm::vec2 vec;

                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texUV = vec;
            }
            else
            {
                vertex.texUV = glm::vec2(0.0f, 0.0f);

            }

            vertices.push_back(vertex);
        }

        for(size_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for(size_t j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<std::string> diffuse_paths;
            std::vector<std::string> normal_paths;
            std::vector<std::string> roughness_paths;

            aiString texturePath;

            for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
                if (material->GetTexture(aiTextureType_DIFFUSE, i, &texturePath) == AI_SUCCESS) {
                    diffuse_paths.push_back(texturePath.C_Str());

                }
            }

            for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_NORMALS); i++) {
                if (material->GetTexture(aiTextureType_NORMALS, i, &texturePath) == AI_SUCCESS) {
                    normal_paths.push_back(texturePath.C_Str());
                }
            }

            for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++) {

                if (material->GetTexture(aiTextureType_SPECULAR, i, &texturePath) == AI_SUCCESS) {
                    roughness_paths.push_back(texturePath.C_Str());
                }
            }

            for(size_t i = 0; i < diffuse_paths.size(); i++)
            {
                diffuse_textures.push_back(Texture2D((_directory + '/' + diffuse_paths[i]).c_str(),
                                                             TextureType::diffuse));
            }

            for(size_t i = 0; i < normal_paths.size(); i++)
            {
                normal_textures.push_back(Texture2D((_directory + '/' + normal_paths[i]).c_str(),
                                                             TextureType::normal));
            }

            for(size_t i = 0; i < roughness_paths.size(); i++)
            {
                roughnes_textures.push_back(Texture2D((_directory + '/' + roughness_paths[i]).c_str(),
                                                             TextureType::roughness));
            }

            LOG_INFORMATION("PATH -> {0}", diffuse_paths[0]);
        }

        return std::make_shared<Mesh>(vertices, indices);
    }

    std::shared_ptr<Material> Model::getMaterial()
    {
        return _material;
    }


    void Model::setMaterial(const std::shared_ptr<Material>& material)
    {
        _material = material;
    }

    std::string Model::getName()
    {
       return _name;
    }

    ObjectType Model::getObjectType()
    {
        return _type;
    }

    bool Model::checkSuccessUpload()
    {
        return _is_loaded;
    }

    void Model::draw(Camera& camera, std::vector<std::unique_ptr<LightObject>>& light_sources)
    {
        if(_material == nullptr)
        {
            LOG_CRITICAL("CUBE HAS NO MATERIAL!!!");
        }
        else
        {
            updateModelMatrix();

            if(_material->getShaderProgram())
            {
                _material->getShaderProgram()->bind();

                _material->getShaderProgram()->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
                _material->getShaderProgram()->setMatrix4("model_matrix", getModelMatrix());
                _material->getShaderProgram()->setVec3("camera_position", camera.getLocation());

                _material->getShaderProgram()->setInt("number_of_point_lights", LightTypeCounter::getNumberOfPointLights());
                _material->getShaderProgram()->setInt("number_of_spot_lights", LightTypeCounter::getNumberOfSpotLights());

                if(light_sources.size() != 0)
                {
                    for(size_t i = 0, p = 0, s = 0; i < light_sources.size(); i++)
                    {
                        if(light_sources[i]->getLightType() == LightType::point)
                        {
                            if(p < LightTypeCounter::getNumberOfPointLights())
                            {
                                std::string uniform_color = "point_light_colors[" + std::to_string(p) + "]";
                                std::string uniform_position = "point_light_positions[" + std::to_string(p) + "]";
                                std::string uniform_constant_attenuation = "constant_attenuation[" + std::to_string(p) + "]";
                                std::string uniform_linear_attenuation= "linear_attenuation[" + std::to_string(p) + "]";

                                _material->getShaderProgram()->setVec3(uniform_color.c_str(), light_sources[i]->getColorVec3());
                                _material->getShaderProgram()->setVec3(uniform_position.c_str(), light_sources[i]->getLocationVec3());
                                _material->getShaderProgram()->setFloat(uniform_constant_attenuation.c_str(),
                                                                        light_sources[i]->getConstantAttenuation());
                                _material->getShaderProgram()->setFloat(uniform_linear_attenuation.c_str(),
                                                                        light_sources[i]->getLinearAttenuation());

                                p++;
                            }
                        }
                        else if(light_sources[i]->getLightType() == LightType::spot)
                        {
                            if(s < LightTypeCounter::getNumberOfSpotLights())
                            {
                                std::string uniform_color = "spot_light_colors[" + std::to_string(s) + "]";
                                std::string uniform_position = "spot_light_positions[" + std::to_string(s) + "]";
                                std::string uniform_direction = "spot_light_direction[" + std::to_string(s) + "]";
                                std::string uniform_outer_cone = "outer_cone[" + std::to_string(s) + "]";
                                std::string uniform_inner_cone = "inner_cone[" + std::to_string(s) + "]";
                                std::string uniform_space_matrix = "spot_light_space_matix[" + std::to_string(s) + "]";

                                _material->getShaderProgram()->setMatrix4(uniform_space_matrix.c_str(), light_sources[i]->getSpaceMatrix());
                                _material->getShaderProgram()->setVec3(uniform_color.c_str(), light_sources[i]->getColorVec3());
                                _material->getShaderProgram()->setVec3(uniform_position.c_str(), light_sources[i]->getLocationVec3());
                                _material->getShaderProgram()->setVec3(uniform_direction.c_str(), light_sources[i]->getDirectionVec3());
                                _material->getShaderProgram()->setFloat(uniform_outer_cone.c_str(), light_sources[i]->getOuterCone());
                                _material->getShaderProgram()->setFloat(uniform_inner_cone.c_str(), light_sources[i]->getInnerCone());

                                s++;
                            }
                        }

                        else if(light_sources[i]->getLightType() == LightType::direction)
                        {
                            _material->getShaderProgram()->setMatrix4("light_space_matrix",light_sources[i]->getSpaceMatrix());
                            _material->getShaderProgram()->setVec3("direction_light_color", light_sources[i]->getColorVec3());
                            _material->getShaderProgram()->setVec3("light_direction", light_sources[i]->getDirectionVec3());
                        }
                        else
                        {
                            LOG_CRITICAL("LIGHT TYPE IS UNKNOWN!!");
                        }
                    }
                }
            }


            for(int i = 0; i < _meshes.size(); i++)
            {
                _material->getShaderProgram()->bind();

                if(i < diffuse_textures.size())
                {
                    _material->getShaderProgram()->setBool("has_color_map", true);
                    diffuse_textures[i].bindUnit(0);
                }
                else
                {
                    _material->getShaderProgram()->setBool("has_color_map", false);
                }

                if(roughnes_textures.size() > i)
                {
                    _material->getShaderProgram()->setBool("has_roughness_map", true);
                    roughnes_textures[i].bindUnit(1);
                }
                else
                {
                    _material->getShaderProgram()->setBool("has_roughness_map", false);
                }


                if(normal_textures.size() > i)
                {
                    _material->getShaderProgram()->setBool("has_normal_map", true);
                    normal_textures[i].bindUnit(1);
                }
                else
                {
                    _material->getShaderProgram()->setBool("has_normal_map", false);
                }

                RenderOpenGL::draw(*_meshes[i]->getVertexArray());

            }
        }
    }

    void Model::draw(std::shared_ptr<ShaderProgram>& shader_program, glm::mat4 space_matrix)
    {
        updateModelMatrix();

        shader_program->bind();
        shader_program->setMatrix4("lightSpaceMatrix", space_matrix);
        shader_program->setMatrix4("model", getModelMatrix());

        for(int i = 0; i < _meshes.size(); i++)
        {
            RenderOpenGL::draw(*_meshes[i]->getVertexArray());
        }
    }

    void Model::draw(std::shared_ptr<ShaderProgram>& shader_program, std::vector<glm::mat4> space_matrices)
    {
        updateModelMatrix();

        shader_program->bind();
        shader_program->setMatrix4("model", getModelMatrix());

        for(size_t i = 0 ; i < space_matrices.size(); i++)
        {
            std::string uniform_shadow_matrices = "shadowMatrices[" + std::to_string(i) + "]";
            shader_program->setMatrix4(uniform_shadow_matrices.c_str(), space_matrices[i]);
        }

        for(int i = 0; i < _meshes.size(); i++)
        {
            RenderOpenGL::draw(*_meshes[i]->getVertexArray());
        }
    }

}
