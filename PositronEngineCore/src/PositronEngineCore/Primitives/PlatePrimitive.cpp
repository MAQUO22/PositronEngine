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
    }



    PlatePrimitive::PlatePrimitive(std::string name)
    {

         std::vector<Vertex> verts(Plate::verteces, Plate::verteces + sizeof(Plate::verteces) / sizeof(Vertex));
         std::vector<GLuint> ind(Plate::indices, Plate::indices + sizeof(Plate::indices) / sizeof(GLuint));

        _name = name;
        _mesh = new Mesh(verts, ind);

    }

    PlatePrimitive::~PlatePrimitive()
    {
        _name = "";
        //delete _mesh;
    }

    void PlatePrimitive::draw(Camera& camera, LightObject& direction_light, std::vector<std::unique_ptr<LightObject>>& point_lights)
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

    Material* PlatePrimitive::getMaterial()
    {
        return _material;
    }

    std::string PlatePrimitive::getName()
    {
        return _name;
    }

    void PlatePrimitive::setMaterial(Material* material)
    {
        _material = material;
    }
}

