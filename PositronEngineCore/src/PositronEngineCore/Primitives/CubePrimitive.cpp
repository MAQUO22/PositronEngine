#include "PositronEngineCore/Primitives/CubePrimitive.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{
    namespace Cube
    {
        Vertex verteces[] =
        {
            //    position                         normal                        UV

            //FRONT
            Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
            Vertex{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

            //BACK
            Vertex{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
            Vertex{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},

            //RIGHT
            Vertex{glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec3( 1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec3( 1.0f, 1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
            Vertex{glm::vec3(-1.0f, 1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

            //LEFT
            Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
            Vertex{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},

            //TOP
            Vertex{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec3( 1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
            Vertex{glm::vec3( 1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

            // BOTTOM
            Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
            Vertex{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
            Vertex{glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
        };

        GLuint indices[] =
        {
            0, 1, 2, // Первый треугольник передней грани
            2, 3, 0, // Второй треугольник передней грани

            4, 5, 6, // Первый треугольник задней грани
            6, 7, 4, // Второй треугольник задней грани

            8, 9, 10, // Первый треугольник правой грани
            10, 11, 8, // Второй треугольник правой грани

            12, 13, 14, // Первый треугольник левой грани
            14, 15, 12, // Второй треугольник левой грани

            16, 17, 18, // Первый треугольник верхней грани
            18, 19, 16, // Второй треугольник верхней грани

            20, 21, 22, // Первый треугольник нижней грани
            22, 23, 20  // Второй треугольник нижней грани
            };
    };

    CubePrimitive::CubePrimitive(std::string name)
    {
         std::vector<Vertex> verts(Cube::verteces, Cube::verteces + sizeof(Cube::verteces) / sizeof(Vertex));
         std::vector<GLuint> ind(Cube::indices, Cube::indices + sizeof(Cube::indices) / sizeof(GLuint));

        _name = name;
        _mesh = std::make_shared<Mesh>(verts, ind);
    }


    CubePrimitive::CubePrimitive(const CubePrimitive& other)
    : _name(other._name),
      _mesh(other._mesh),
      _material(other._material)
    {

    }

    CubePrimitive& CubePrimitive::operator=(const CubePrimitive& other)
    {
        if (this != &other)
        {
            _name = other._name;
            _mesh = other._mesh;
            _material = other._material;
        }
        return *this;
    }


    void CubePrimitive::draw(Camera& camera,std::vector<std::unique_ptr<LightObject>>& light_sources)
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

                _material->getShaderProgram()->setFloat("ambient_factor", _material->getLightConfig().ambient);
                _material->getShaderProgram()->setFloat("diffuse_factor", _material->getLightConfig().diffuse);
                _material->getShaderProgram()->setFloat("shininess", _material->getLightConfig().shininess);
                _material->getShaderProgram()->setFloat("specular_factor", _material->getLightConfig().specular);

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

            RenderOpenGL::draw(*_mesh->getVertexArray());

            for(size_t i = 0; i < _material->getTexturesVector().size(); i++)
                _material->getTexturesVector()[i].unbindUnit();
        }
    }

    void CubePrimitive::draw(std::shared_ptr<ShaderProgram>& shader_program, glm::mat4 space_matrix)
    {
        updateModelMatrix();

        shader_program->bind();
        shader_program->setMatrix4("lightSpaceMatrix", space_matrix);
        shader_program->setMatrix4("model", getModelMatrix());

        RenderOpenGL::draw(*_mesh->getVertexArray());
    }

    std::shared_ptr<Material> CubePrimitive::getMaterial()
    {
        return _material;
    }

    std::shared_ptr<Mesh> CubePrimitive::getMesh()
    {
        return _mesh;
    }

    std::string CubePrimitive::getName()
    {
        return _name;
    }

    void CubePrimitive::setMaterial(const std::shared_ptr<Material>& material)
    {
        _material = material;
    }
}
