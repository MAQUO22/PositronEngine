#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "PositronEngineCore/VertexArray.hpp"
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
            Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices);

            Mesh() = delete;
            ~Mesh();

            VertexArray* getVertexArray();

        private:
            std::vector<Vertex> _vertices;
            std::vector<GLuint> _indices;

            VertexArray* _VAO = nullptr;
            VertexBuffer* _positions_buffer = nullptr;
            IndexBuffer* _indices_buffer = nullptr;

    };
}
