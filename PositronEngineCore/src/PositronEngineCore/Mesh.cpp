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

        _positions_buffer = std::make_shared<VertexBuffer>(Mesh::_vertices.data(), Mesh::_vertices.size() * sizeof(Vertex), mesh);
        _indices_buffer = std::make_shared<IndexBuffer>(Mesh::_indices.data(), Mesh::_indices.size());

        _VAO = std::make_shared<VertexArray>();
        _VAO->addVertexBuffer(_positions_buffer);
        _VAO->setIndexBuffer(_indices_buffer);
    }

    Mesh::Mesh(Mesh& other)
        : _vertices(std::move(other._vertices)),
          _indices(std::move(other._indices)),
          _VAO(other._VAO),
          _positions_buffer(other._positions_buffer),
          _indices_buffer(other._indices_buffer)
    {
        other._VAO.reset();
        other._positions_buffer.reset();
        other._indices_buffer.reset();
    }

    Mesh::~Mesh()
    {
        _VAO.reset();
        _positions_buffer.reset();
        _indices_buffer.reset();
    }


    Mesh& Mesh::operator=(Mesh& other)
    {
        if (this != &other) {
            _vertices = std::move(other._vertices);
            _indices = std::move(other._indices);
            _VAO = other._VAO;
            _positions_buffer = other._positions_buffer;
            _indices_buffer = other._indices_buffer;

            other._VAO.reset();
            other._positions_buffer.reset();
            other._indices_buffer.reset();
        }
        return *this;
    }

    std::shared_ptr<VertexArray> Mesh::getVertexArray()
    {
        return _VAO;
    }
}
