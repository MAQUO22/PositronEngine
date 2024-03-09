#include "PositronEngineCore/Primitives/CubePrimitive.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"

namespace PositronEngine
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


    CubePrimitive::CubePrimitive(std::string name)
    {
         Texture2D textures[]
         {
             Texture2D("wood.jpg", TextureType::diffuse)
         };

         std::vector<Vertex> verts(verteces, verteces + sizeof(verteces) / sizeof(Vertex));
         std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
         std::vector<Texture2D> tex(textures, textures + sizeof(textures) / sizeof(Texture2D));

        _name = name;
        _mesh = new Mesh(verts, ind, tex);

    }

    CubePrimitive::~CubePrimitive()
    {
        _name = "";
        delete _mesh;
    }

    void CubePrimitive::draw()
    {
        updateModelMatrix();
        _mesh->textures[0].bindUnit(0);
        RenderOpenGL::draw(*_mesh->VAO);
        _mesh->textures[0].unbindUnit();
    }
}
