#include "PositronEngineCore/Primitives/SpherePrimitive.hpp"
#include "PositronEngineCore/Sphere.h"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{
    namespace Sphere
    {
        class Sphere temp(1.0f, 36, 18, true, 3);

        std::vector<Vertex> vertices;


        void floatToVertex(std::vector<Vertex>& vertices)
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
                vertices.push_back(Vertex{
                    glm::vec3(sphereVertices[i], sphereVertices[i + 1], sphereVertices[i + 2]), // позиция
                    glm::vec3(sphereNormals[i], sphereNormals[i + 1], sphereNormals[i + 2]),    // нормаль
                    glm::vec2(sphereTexCoords[j], sphereTexCoords[j + 1])                       // текстурные координаты
                });
            }
        }
    };


    SpherePrimitive::SpherePrimitive(std::string name)
    {
        Sphere::floatToVertex(Sphere::vertices);

        std::vector<GLuint> ind;
        for(size_t i = 0; i < Sphere::temp.getIndexCount(); i++)
            ind.push_back(Sphere::temp.getIndices()[i]);

        _name = name;
        _mesh = new Mesh(Sphere::vertices, ind);

    }

    SpherePrimitive::~SpherePrimitive()
    {
        _name = "";
        delete _mesh;
    }

    void SpherePrimitive::draw(Camera& camera, LightObject& direction_light, std::vector<std::unique_ptr<LightObject>>& point_lights)
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

    Material* SpherePrimitive::getMaterial()
    {
        return _material;
    }

    void SpherePrimitive::setMaterial(Material* material)
    {
        _material = material;
    }

    std::string SpherePrimitive::getName()
    {
        return _name;
    }

    Mesh* SpherePrimitive::getMesh()
    {
        return _mesh;
    }
}

