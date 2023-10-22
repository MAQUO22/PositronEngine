#include "Planet.hpp"

namespace PositronEngine
{
    float* PositronEngine::Planet::getLocation()
    {
        return _location;
    }

    float* PositronEngine::Planet::getRotation()
    {
        return _rotation;
    }

    float* PositronEngine::Planet::getScale()
    {
        return _scale;
    }

    VertexArray* Planet::getVertexArrayObject()
    {
        return _vertex_array_object;
    }

    Texture2D* Planet::getTexture()
    {
        return _texture;
    }

    void Planet::setTexture(const char* path)
    {
        _texture = new Texture2D(path);
    }

    void PositronEngine::Planet::setVertexArrayObject()
    {
        _vertex_array_object = new VertexArray();

        _vertex_buffer_position = new VertexBuffer(Planet::getInterleavedVertices(),
                                                  Planet::getInterleavedVertexSize(),
                                                  _planet_layout);

        _vertex_buffer_normal = new VertexBuffer(Planet::getNormals(),
                                                Planet::getNormalSize(),
                                                _planet_layout);

        _vertex_buffer_texCoords = new VertexBuffer(Planet::getTexCoords(),
                                                   Planet::getTexCoordSize(),
                                                   _planet_layout);

        _index_buffer = new IndexBuffer(Planet::getIndices(), Planet::getLineIndexSize());

        _vertex_array_object->addVertexBuffer(*_vertex_buffer_position);
        _vertex_array_object->addVertexBuffer(*_vertex_buffer_normal);
        _vertex_array_object->addVertexBuffer(*_vertex_buffer_texCoords);
        _vertex_array_object->setIndexBuffer(*_index_buffer);
    }
}
