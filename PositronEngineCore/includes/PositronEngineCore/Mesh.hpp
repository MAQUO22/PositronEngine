#pragma once

#include <string>
#include <glad/glad.h>
#include "PositronEngineCore/VertexArray.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/Texture2D.hpp"


namespace PositronEngine
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texUV;
    };

    class Mesh
    {
        public:
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;
            std::vector<Texture2D> textures;

            VertexArray* VAO = nullptr;
            VertexBuffer* positionB = nullptr;
            IndexBuffer* indicesB = nullptr;

            Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture2D> &textures);
            Mesh() = delete;
            ~Mesh();

    };
}
