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
        _mesh = new Mesh(verts, ind);

    }

    CubePrimitive::~CubePrimitive()
    {
        _name = "";
        delete _mesh;
    }


    void CubePrimitive::draw(Camera& camera, LightObject& direction_light, std::vector<std::unique_ptr<LightObject>>& point_lights)
    {
        if(_material == nullptr)
        {
            LOG_CRITICAL("CUBE HAS NO MATERIAL!!!");
        }

        else
        {
            updateModelMatrix();

            _material->getShaderProgram()->bind();
            _material->getShaderProgram()->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
            _material->getShaderProgram()->setVec3("direction_light_color", direction_light.getColorVec3());

            _material->getShaderProgram()->setFloat("ambient_factor", _material->getLightConfig().ambient);
            _material->getShaderProgram()->setFloat("diffuse_factor", _material->getLightConfig().diffuse);
            _material->getShaderProgram()->setVec3("camera_position", camera.getLocation());
            _material->getShaderProgram()->setVec3("light_direction", direction_light.getDirectionVec3());

            _material->getShaderProgram()->setFloat("shininess", _material->getLightConfig().shininess);
            _material->getShaderProgram()->setFloat("specular_factor", _material->getLightConfig().specular);
            _material->getShaderProgram()->setMatrix4("model_matrix", getModelMatrix());

            _material->getShaderProgram()->setInt("number_of_point_lights", LightTypeCounter::getNumberOfPointLights());

            for(size_t i = 0; i < LightTypeCounter::getNumberOfPointLights(); i++)
            {
                std::string uniform_color = "point_light_colors[" + std::to_string(i) + "]";
                std::string uniform_position = "point_light_positions[" + std::to_string(i) + "]";
                _material->getShaderProgram()->setVec3(uniform_color.c_str(), point_lights[i]->getColorVec3());
                _material->getShaderProgram()->setVec3(uniform_position.c_str(), point_lights[i]->getLocationVec3());
            }

            for(size_t i = 0; i < _material->getTexturesVector().size(); i++)
            {

                if(_material->getTexturesVector()[i].getTextureType() == TextureType::diffuse)
                {
                    _material->getTexturesVector()[i].bindUnit(0);
                }

                else if(_material->getTexturesVector()[i].getTextureType() == TextureType::specular)
                {
                    _material->getTexturesVector()[i].bindUnit(1);
                }

                else if(_material->getTexturesVector()[i].getTextureType() == TextureType::normal)
                {
                    _material->getTexturesVector()[i].bindUnit(2);
                }
            }

            RenderOpenGL::draw(*_mesh->getVertexArray());

            for(size_t i = 0; i < _material->getTexturesVector().size(); i++)
                _material->getTexturesVector()[i].unbindUnit();
        }
    }

    Material* CubePrimitive::getMaterial()
    {
        return _material;
    }

    void CubePrimitive::setMaterial(Material* material)
    {
        _material = material;
    }
}
