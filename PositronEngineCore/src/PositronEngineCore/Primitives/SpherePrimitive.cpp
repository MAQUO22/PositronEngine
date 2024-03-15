#include "PositronEngineCore/Primitives/SpherePrimitive.hpp"
#include "PositronEngineCore/Sphere.h"
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
        Texture2D textures[]
        {
            Texture2D("sun.bmp", TextureType::diffuse),
        };

        Sphere::floatToVertex(Sphere::vertices);

        std::vector<GLuint> ind;
        for(size_t i = 0; i < Sphere::temp.getIndexCount(); i++)
            ind.push_back(Sphere::temp.getIndices()[i]);

        std::vector<Texture2D> tex(textures, textures + sizeof(textures) / sizeof(Texture2D));

        _name = name;
        _mesh = new Mesh(Sphere::vertices, ind, tex);

    }

    SpherePrimitive::~SpherePrimitive()
    {
        _name = "";
        delete _mesh;
    }

    void SpherePrimitive::draw()
    {
        updateModelMatrix();
        _mesh->textures[0].bindUnit(0);
        RenderOpenGL::draw(*_mesh->VAO);
        _mesh->textures[0].unbindUnit();
    }
}

