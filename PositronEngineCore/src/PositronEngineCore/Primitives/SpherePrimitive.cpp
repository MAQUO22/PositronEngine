#include "PositronEngineCore/Primitives/SpherePrimitive.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{
    namespace SphereMesh
    {
        void floatToVertex(std::vector<Vertex>& vertices, Sphere& temp)
        {
            const float* sphereVerticesPtr = temp.getVertices();
            const float* sphereNormalsPtr = temp.getNormals();
            const float* sphereTexCoordsPtr = temp.getTexCoords();

            size_t numVertices = temp.getVertexCount();
            size_t numNormals = temp.getNormalCount();
            size_t numTexCoords = temp.getTexCoordCount();


            std::vector<float> sphereVertices(sphereVerticesPtr, sphereVerticesPtr + numVertices * 3);
            std::vector<float> sphereNormals(sphereNormalsPtr, sphereNormalsPtr + numNormals * 3);
            std::vector<float> sphereTexCoords(sphereTexCoordsPtr, sphereTexCoordsPtr + numTexCoords * 2);

            for (size_t i = 0, j = 0; i < sphereVertices.size(); i += 3, j += 2) {
                vertices.emplace_back(Vertex{
                    glm::vec3(sphereVertices[i], sphereVertices[i + 1], sphereVertices[i + 2]), // позиция
                    glm::vec3(sphereNormals[i], sphereNormals[i + 1], sphereNormals[i + 2]),    // нормаль
                    glm::vec2(sphereTexCoords[j], sphereTexCoords[j + 1])                       // текстурные координаты
                });
            }
        }
    };


    SpherePrimitive::SpherePrimitive(std::string name)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> ind;
        _sphere = std::make_unique<Sphere>(1.0f, 144, 72, true, 3);

        SphereMesh::floatToVertex(vertices, *_sphere);
        std::copy(_sphere->getIndices(), _sphere->getIndices() + _sphere->getIndexCount(), std::back_inserter(ind));

        _name = name;
        _mesh = std::make_shared<Mesh>(vertices, ind);
    }

    SpherePrimitive::SpherePrimitive(const SpherePrimitive& other)
    : _name(other._name),
      _sphere(std::make_unique<Sphere>(*other._sphere)),
      _mesh(other._mesh),
      _material(other._material)
    {

    }

    SpherePrimitive& SpherePrimitive::operator=(const SpherePrimitive& other)
    {
        if (this != &other)
        {
            _name = other._name;
            _sphere = std::make_unique<Sphere>(*other._sphere);
            _mesh = other._mesh;
            _material = other._material;
        }
        return *this;
    }

    void SpherePrimitive::draw(Camera& camera,std::vector<std::unique_ptr<LightObject>>& light_sources)
    {
        if(_material == nullptr)
        {
            LOG_CRITICAL("SPHERE HAS NO MATERIAL!!!");
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

            _material->bindMaterialMaps();

            RenderOpenGL::draw(*_mesh->getVertexArray());

            _material->unbindMaterialMaps();
        }
    }

    void SpherePrimitive::draw(std::shared_ptr<ShaderProgram>& shader_program, glm::mat4 space_matrix)
    {
        updateModelMatrix();

        shader_program->bind();
        shader_program->setMatrix4("lightSpaceMatrix", space_matrix);
        shader_program->setMatrix4("model", getModelMatrix());

        RenderOpenGL::draw(*_mesh->getVertexArray());
    }

    void SpherePrimitive::draw(std::shared_ptr<ShaderProgram>& shader_program, std::vector<glm::mat4> space_matrices)
    {
        updateModelMatrix();

        shader_program->bind();
        shader_program->setMatrix4("model", getModelMatrix());

        for(size_t i = 0 ; i < space_matrices.size(); i++)
        {
            std::string uniform_shadow_matrices = "shadowMatrices[" + std::to_string(i) + "]";
            shader_program->setMatrix4(uniform_shadow_matrices.c_str(), space_matrices[i]);
        }

        RenderOpenGL::draw(*_mesh->getVertexArray());
    }

    std::shared_ptr<Material> SpherePrimitive::getMaterial()
    {
        return _material;
    }

    std::shared_ptr<Mesh> SpherePrimitive::getMesh()
    {
        return _mesh;
    }

    void SpherePrimitive::setMaterial(const std::shared_ptr<Material>& material)
    {
        _material = material;
    }

    std::string SpherePrimitive::getName()
    {
        return _name;
    }
}

