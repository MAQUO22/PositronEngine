#include "PositronEngineCore/Primitives/PlatePrimitive.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"

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
         Texture2D textures[]
         {
             Texture2D("wood.jpg", TextureType::diffuse),
             Texture2D("wood_specular.jpg", TextureType::specular),
             Texture2D("wood_normal.jpg", TextureType::normal),
         };

         std::vector<Vertex> verts(Plate::verteces, Plate::verteces + sizeof(Plate::verteces) / sizeof(Vertex));
         std::vector<GLuint> ind(Plate::indices, Plate::indices + sizeof(Plate::indices) / sizeof(GLuint));
         std::vector<Texture2D> tex(textures, textures + sizeof(textures) / sizeof(Texture2D));

        _name = name;
        _mesh = new Mesh(verts, ind, tex);

    }

    PlatePrimitive::~PlatePrimitive()
    {
        _name = "";
        delete _mesh;
    }

    void PlatePrimitive::draw()
    {
        updateModelMatrix();
        _mesh->textures[0].bindUnit(0);
        _mesh->textures[1].bindUnit(1);
        _mesh->textures[2].bindUnit(2);
        RenderOpenGL::draw(*_mesh->VAO);
        _mesh->textures[2].unbindUnit();
        _mesh->textures[1].unbindUnit();
        _mesh->textures[0].unbindUnit();
    }
}

