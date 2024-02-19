#include "PositronEngineCore/Mesh.hpp"

namespace PositronEngine
{
    BufferLayout mesh
    {
        ShaderDataType::Float3,
        ShaderDataType::Float3,
        ShaderDataType::Float2
    };


    Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices,  std::vector<Texture2D> &textures)
    {
        Mesh::vertices = std::move(vertices);
        Mesh::indices = std::move(indices);
        Mesh::textures = std::move(textures);

        positionB = new VertexBuffer(Mesh::vertices.data(), Mesh::vertices.size() * sizeof(Vertex), mesh);
        indicesB = new IndexBuffer(Mesh::indices.data(), Mesh::indices.size() * sizeof(GLuint));

        VAO = new VertexArray();
        VAO->addVertexBuffer(*positionB);
        VAO->setIndexBuffer(*indicesB);
    }

    Mesh::~Mesh()
    {
        delete VAO;
        delete positionB;
        delete indicesB;
    }
}
