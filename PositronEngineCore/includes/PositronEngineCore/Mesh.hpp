#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "PositronEngineCore/VertexArray.hpp"
#include "PositronEngineCore/Texture2D.hpp"


namespace PositronEngine
{
    struct BufferLayoutl;
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

            Mesh(Mesh& other);
            Mesh& operator=(Mesh& other);

            Mesh();
            ~Mesh();

            std::shared_ptr<VertexArray> getVertexArray();

        private:
            std::vector<Vertex> _vertices;
            std::vector<GLuint> _indices;

            std::shared_ptr<VertexArray> _VAO;
            std::shared_ptr<VertexBuffer> _positions_buffer;
            std::shared_ptr<IndexBuffer> _indices_buffer;

    };
}
