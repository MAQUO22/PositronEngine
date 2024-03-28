#include "PositronEngineCore/Mesh.hpp"

namespace PositronEngine
{
    BufferLayout mesh
    {
        ShaderDataType::Float3,
        ShaderDataType::Float3,
        ShaderDataType::Float2
    };

    Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
    {
        Mesh::_vertices = std::move(vertices);
        Mesh::_indices = std::move(indices);

        _positions_buffer = new VertexBuffer(Mesh::_vertices.data(), Mesh::_vertices.size() * sizeof(Vertex), mesh);
        _indices_buffer = new IndexBuffer(Mesh::_indices.data(), Mesh::_indices.size() * sizeof(GLuint));

        _VAO = new VertexArray();
        _VAO->addVertexBuffer(*_positions_buffer);
        _VAO->setIndexBuffer(*_indices_buffer);
    }

    Mesh::~Mesh()
    {
        delete _VAO;
        delete _positions_buffer;
        delete _indices_buffer;
    }

    VertexArray* Mesh::getVertexArray()
    {
        return _VAO;
    }
}
