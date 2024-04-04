#include "PositronEngineCore/Primitives/Model.hpp"
#include "PositronEngineCore/Log.hpp"

#include <fstream>
#include <PositronEngineCore/RenderOpenGL.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PositronEngine
{

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


    std::vector<unsigned char> Model::getData()
    {
        std::string bytes_text;
        std::string uri = _json["buffers"][0]["uri"];

        std::string file_str = std::string(_file);
        std::string file_directory = file_str.substr(0, file_str.find_last_of('/') + 1);
        bytes_text = getFileContext((file_directory + uri).c_str());

        std::vector<unsigned char> data(bytes_text.begin(), bytes_text.end());

        return data;
    }

    Model::Model(const char* file_name)
    {
        std::string file_context = getFileContext(file_name);
        _json = JSON::parse(file_context);

        Model::_file = file_name;
        _data = getData();

        LOG_INFORMATION("MODEL CONSTRUCTOR");
        traverseNode(0);
    }

    glm::mat4 Model::getModelMatrix(int index)
    {
        if(index < matrices_meshes.size())
        {
            return matrices_meshes[index];
        }
        else
        {
            LOG_CRITICAL("WRONG INDEX!");
            return glm::mat4(1.0f);
        }
    }

    std::vector<float> Model::getFloats(JSON accessor)
    {
        std::vector<float> float_vec;

        unsigned int buffer_view_indices = accessor.at("bufferView").get<unsigned int>();
        unsigned int count = accessor["count"];
        unsigned int accessor_byte_offset = accessor.value("byteOffset", 0);
        std::string type = accessor["type"];

        JSON buffer_view = _json["bufferViews"][buffer_view_indices];
        unsigned int byte_offset = buffer_view["byteOffset"];


        unsigned int number_per_verteces;
        if(type == "SCALAR") number_per_verteces = 1;
        else if(type == "VEC2") number_per_verteces = 2;
        else if(type == "VEC3") number_per_verteces = 3;
        else if(type == "VEC4") number_per_verteces = 4;
        else LOG_CRITICAL("TYPE IS INVALID (NOT SCALAR OR VEC)");

        unsigned int beggining_of_data = byte_offset + accessor_byte_offset;
        unsigned int length_of_data = count * 4 * number_per_verteces;

        for(size_t i = beggining_of_data; i < beggining_of_data + length_of_data; i)
        {
            unsigned char bytes[] = {_data[i++], _data[i++], _data[i++], _data[i++] };
            float value;
            std::memcpy(&value, bytes, sizeof(float));
            float_vec.push_back(value);
        }

        return float_vec;
    }

    std::vector<GLuint> Model::getIndices(JSON accessor)
    {
        std::vector<GLuint> indices;

        unsigned int buffer_view_indices = accessor.at("bufferView").get<unsigned int>();
        unsigned int count = accessor["count"];
        unsigned int accessor_byte_offset = accessor.value("byteOffset", 0);
        unsigned int component_type = accessor["componentType"];


        JSON buffer_view = _json["bufferViews"][buffer_view_indices];
        unsigned int byte_offset = buffer_view.value("byteOffset", 0);

        unsigned int beggining_of_data = byte_offset + accessor_byte_offset;

        if(component_type == 5125)
        {
            for(size_t i = beggining_of_data; i < byte_offset + accessor_byte_offset + count * 4; i)
            {
                unsigned char bytes[] = { _data[i++], _data[i++], _data[i++], _data[i++] };
                unsigned int value;
                std::memcpy(&value, bytes, sizeof(unsigned int));
                indices.push_back((GLuint)value);
            }
        }
        else if(component_type == 5123)
        {
            for(size_t i = beggining_of_data; i < byte_offset + accessor_byte_offset + count * 2; i)
            {
                unsigned char bytes[] = { _data[i++], _data[i++]};
                unsigned int value;
                std::memcpy(&value, bytes, sizeof(unsigned short));
                indices.push_back((GLuint)value);
            }
        }
        else if(component_type == 5122)
        {
            for(size_t i = beggining_of_data; i < byte_offset + accessor_byte_offset + count * 2; i)
            {
                unsigned char bytes[] = { _data[i++], _data[i++]};
                unsigned int value;
                std::memcpy(&value, bytes, sizeof(short));
                indices.push_back((GLuint)value);
            }
        }

        return indices;
    }

    std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions,
                                                std::vector<glm::vec3> normals,
                                                std::vector<glm::vec2> texture_uvs)
    {
        std::vector<Vertex> vertices;

        for(size_t i = 0; i < positions.size(); i++)
        {
            vertices.push_back(Vertex{ positions[i], normals[i], texture_uvs[i] });
        }

        return vertices;
    }


    void Model::loadMesh(unsigned int index_mesh)
    {
        LOG_INFORMATION("MODEL LOAD MESH");
        unsigned int position_accessor_index = _json["meshes"][index_mesh]["primitives"][0]["attributes"]["POSITION"];
        unsigned int normal_accessor_index = _json["meshes"][index_mesh]["primitives"][0]["attributes"]["NORMAL"];
        unsigned int texture_uv_accessor_index = _json["meshes"][index_mesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
        unsigned int index_accessor_index = _json["meshes"][index_mesh]["primitives"][0]["indices"];

        std::vector<float> position_vector = getFloats(_json["accessors"][position_accessor_index]);
        std::vector<glm::vec3> positions = groupFloatsVec3(position_vector);

        LOG_INFORMATION("POSITIONS MESH");

        std::vector<float> normal_vector = getFloats(_json["accessors"][normal_accessor_index]);
        std::vector<glm::vec3> normals = groupFloatsVec3(normal_vector);

        LOG_INFORMATION("NORMALS MESH");

        std::vector<float> texture_uvs_vector = getFloats(_json["accessors"][texture_uv_accessor_index]);
        std::vector<glm::vec2> texture_uvs = groupFloatsVec2(texture_uvs_vector);

        LOG_INFORMATION("TEX COORDS MESH");

        std::vector<Vertex> vertices = assembleVertices(positions, normals, texture_uvs);
        std::vector<GLuint> indices = getIndices(_json["accessors"][index_accessor_index]);

        _meshes.push_back(std::make_shared<Mesh>(vertices, indices));

    }

    void Model::traverseNode(unsigned int next_node, glm::mat4 matrix)
    {
        JSON node = _json["nodes"][next_node];

        glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
        if(node.find("translation") != node.end())
        {
            float translation_values[3];
            for(size_t i = 0; i < node["translation"].size(); i++)
            {
                translation_values[i] = (node["translation"][i]);
            }
            translation = glm::make_vec3(translation_values);
        }

        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        if(node.find("rotation") != node.end())
        {
            float rotation_values[4] =
            {
                node["rotation"][3],
                node["rotation"][0],
                node["rotation"][1],
                node["rotation"][2]
            };

            rotation = glm::make_quat(rotation_values);
        }

        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
        if(node.find("scale") != node.end())
        {
            float scale_values[3];
            for(size_t i = 0; i < node["scale"].size(); i++)
            {
                scale_values[i] = (node["scale"][i]);
            }
            scale = glm::make_vec3(scale_values);
        }

        glm::mat4 matrix_node = glm::mat4(1.0f);
        if(node.find("matrix") != node.end())
        {
            float matrix_values[16];
            for(size_t i = 0; i < node["matrix"].size(); i++)
            {
                matrix_values[i] = (node["matrix"][i]);
            }
            matrix_node = glm::make_mat4(matrix_values);
        }

        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 rot = glm::mat4(1.0f);
        glm::mat4 sca = glm::mat4(1.0f);

        trans = glm::translate(trans, translation);
        rot = glm::mat4_cast(rotation);
        sca = glm::scale(sca, scale);

        glm::mat4 matrix_next_node = matrix * matrix_node * trans * rot * sca;

        if(node.find("mesh") != node.end())
        {
            translations_meshes.push_back(translation);
            rotations_meshes.push_back(rotation);
            scales_meshes.push_back(scale);
            matrices_meshes.push_back(matrix_next_node);

            loadMesh(node["mesh"]);
        }

        if(node.find("children") != node.end())
        {
            for(size_t i = 0; i < node["children"].size(); i++)
            {
                traverseNode(node["children"][i], matrix_next_node);
            }
        }
    }

    std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> float_vec)
    {
        std::vector<glm::vec2> vectors;

        for(size_t i = 0; i < float_vec.size(); i)
        {
            vectors.push_back(glm::vec2(float_vec[i++], float_vec[i++]));
        }

        return vectors;
    }

    std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> float_vec)
    {
        std::vector<glm::vec3> vectors;

        for(size_t i = 0; i < float_vec.size(); i)
        {
            vectors.push_back(glm::vec3(float_vec[i++], float_vec[i++], float_vec[i++]));
        }

        return vectors;
    }

    std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> float_vec)
    {
        std::vector<glm::vec4> vectors;

        for(size_t i = 0; i < float_vec.size(); i)
        {
            vectors.push_back(glm::vec4(float_vec[i++], float_vec[i++], float_vec[i++], float_vec[i++]));
        }

        return vectors;
    }

    std::shared_ptr<Material> Model::getMaterial()
    {
        return _material;
    }


    void Model::setMaterial(const std::shared_ptr<Material>& material)
    {
        _material = material;
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
                _material->getShaderProgram()->setMatrix4("model_matrix", GameObject::getModelMatrix());

                _material->getShaderProgram()->setVec3("camera_position", camera.getLocation());

                _material->getShaderProgram()->setFloat("ambient_factor", _material->getLightConfig().ambient);
                _material->getShaderProgram()->setFloat("diffuse_factor", _material->getLightConfig().diffuse);
                _material->getShaderProgram()->setFloat("shininess", _material->getLightConfig().shininess);
                _material->getShaderProgram()->setFloat("specular_factor", _material->getLightConfig().specular);

                _material->getShaderProgram()->setInt("number_of_point_lights", LightTypeCounter::getNumberOfPointLights());
                _material->getShaderProgram()->setInt("number_of_spot_lights", LightTypeCounter::getNumberOfSpotLights());
                _material->getShaderProgram()->setBool("model", false);

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

            for(size_t i = 0; i < _material->getTexturesVector().size(); i++)
            {

                if(_material->getTexturesVector()[i].getType() == TextureType::diffuse)
                {
                    _material->getTexturesVector()[i].bindUnit(0);
                }

                else if(_material->getTexturesVector()[i].getType() == TextureType::specular)
                {
                    _material->getTexturesVector()[i].bindUnit(1);
                }

                else if(_material->getTexturesVector()[i].getType() == TextureType::normal)
                {
                    _material->getTexturesVector()[i].bindUnit(2);
                }
            }

            LOG_INFORMATION("MESHES SIZE -> {0}", _meshes.size());
            for(int i = 0; i < _meshes.size(); i++)
            {
                _material->getShaderProgram()->bind();
                _material->getShaderProgram()->setMatrix4("model_matrix", matrices_meshes[i]);

                RenderOpenGL::draw(*_meshes[i]->getVertexArray());
            }

            for(size_t i = 0; i < _material->getTexturesVector().size(); i++)
                    _material->getTexturesVector()[i].unbindUnit();
        }
    }

    void Model::draw(std::shared_ptr<ShaderProgram>& shader_program, glm::mat4 space_matrix) {}

}
