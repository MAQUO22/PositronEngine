#include "PositronEngineCore/Primitives/PlatePrimitive.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{
    namespace Plate
    {
       Vertex verteces[] =
        {
          //position                               normal                        UV
            Vertex{glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec3(0.0f,  1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec3(0.0f,  1.0f,  1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
            Vertex{glm::vec3(0.0f, -1.0f,  1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        };

        GLuint indices[] =
        {
            0, 1, 2, // Первый треугольник
            2, 3, 0, // Второй треугольник
        };

        int kekes = 0;
    }



    PlatePrimitive::PlatePrimitive(std::string name)
    {

         std::vector<Vertex> verts(Plate::verteces, Plate::verteces + sizeof(Plate::verteces) / sizeof(Vertex));
         std::vector<GLuint> ind(Plate::indices, Plate::indices + sizeof(Plate::indices) / sizeof(GLuint));

        _name = name;
        _type = ObjectType::primitive;
        _mesh = std::make_unique<Mesh>(verts, ind);

    }

    PlatePrimitive::PlatePrimitive(const PlatePrimitive& other)
    : _name(other._name),
      _mesh(std::make_unique<Mesh>(*other._mesh)),
      _material(other._material)
    {
    }

    PlatePrimitive& PlatePrimitive::operator=(const PlatePrimitive& other)
    {
        if (this != &other)
        {
            _name = other._name;
            _mesh = std::make_unique<Mesh>(*other._mesh);
            _material = other._material;
        }
        return *this;
    }

    void PlatePrimitive::draw(Camera& camera,std::vector<std::unique_ptr<LightObject>>& light_sources)
    {
        if(_material == nullptr)
        {
            LOG_CRITICAL("PLATE HAS NO MATERIAL!!!");
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

    void PlatePrimitive::draw(std::shared_ptr<ShaderProgram>& shader_program, glm::mat4 space_matrix)
    {
        updateModelMatrix();

        shader_program->bind();
        shader_program->setMatrix4("lightSpaceMatrix", space_matrix);
        shader_program->setMatrix4("model", getModelMatrix());

        RenderOpenGL::draw(*_mesh->getVertexArray());
    }

    void PlatePrimitive::draw(std::shared_ptr<ShaderProgram>& shader_program, std::vector<glm::mat4> space_matrices)
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

    std::shared_ptr<Material> PlatePrimitive::getMaterial()
    {
        return _material;
    }

    std::unique_ptr<Mesh> PlatePrimitive::getMesh()
    {
        return std::move(_mesh);
    }

    std::string PlatePrimitive::getName()
    {
        return _name;
    }

    ObjectType PlatePrimitive::getObjectType()
    {
        return _type;
    }

    void PlatePrimitive::setMaterial(const std::shared_ptr<Material>& material)
    {
        _material = material;
    }
}

