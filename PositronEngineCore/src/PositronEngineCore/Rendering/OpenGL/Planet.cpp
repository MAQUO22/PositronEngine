#include "PositronEngineCore/Planet.hpp"

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

    Texture2D* Planet::getTexture(const int id)
    {
        return _textures_vector[id];
    }

    glm::mat4 Planet::getModelMatrix()
    {
        return _model_matrix;
    }

    float Planet::getOrbitRadius()
    {
        return _orbit_radius;
    }

    float Planet::getOrbitSpeed()
    {
        return _orbit_speed;
    }

    float Planet::getAngle()
    {
        return _angle;
    }


    void Planet::setModelMatrix(glm::mat4 new_model_matrix)
    {
        _model_matrix = new_model_matrix;
    }

    void Planet::updateMatrix()
    {
        glm::mat4 location_matrix(1,                    0,                  0,               0,
                                  0,                    1,                  0,               0,
                                  0,                    0,                  1,               0,
                                  _location[0],          _location[1],      _location[2],    1);


            glm::mat4 _rotate_matrix_x(1,    0,                                   0,                                   0,
                                       0,    cos(glm::radians(_rotation[0])),     sin(glm::radians(_rotation[0])),     0,
                                       0,    -sin(glm::radians(_rotation[0])),    cos(glm::radians(_rotation[0])),     0,
                                       0,    0,                                   0,                                   1);


            glm::mat4 _rotate_matrix_y(cos(glm::radians(_rotation[1])),       0,      -sin(glm::radians(_rotation[1])),    0,
                                       0,                                     1,      0,                                   0,
                                       sin(glm::radians(_rotation[1])),       0,      cos(glm::radians(_rotation[1])),     0,
                                       0,                                     0,      0,                                   1);


            glm::mat4 _rotate_matrix_z(cos(glm::radians(_rotation[2])),       sin(glm::radians(_rotation[2])),     0,      0,
                                       -sin(glm::radians(_rotation[2])),      cos(glm::radians(_rotation[2])),     0,      0,
                                       0,                                     0,                                   1,      0,
                                       0,                                     0,                                   0,      1);

            glm::mat4x4 _scale_matrix(_scale[0],  0,              0,          0,
                                      0,          _scale[1],      0,          0,
                                      0,          0,              _scale[2],  0,
                                      0,          0,              0,          1);

            _model_matrix = location_matrix * _rotate_matrix_x * _rotate_matrix_y * _rotate_matrix_z * _scale_matrix;
    }

    void Planet::addRotation(const float vel)
    {
        _rotation[2] -= vel;
    }

    void Planet::addAngle()
    {
        _angle += _orbit_speed;
    }

    void Planet::doOrbitalMotion(const float* location)
    {
        float e_x = location[0] + _orbit_radius * cos(_angle);
        float e_y = location[1] + _orbit_radius * sin(_angle);

        setLocation(e_x, e_y, _location[2]);
    }

    void Planet::addTexture(const char* path)
    {;
        _textures_vector.push_back(new Texture2D(path));
    }

    void Planet::setLocation(const float x, const float y, const float z)
    {
        _location[0] = x;
        _location[1] = y;
        _location[2] = z;
    }

    void Planet::setRotation(const float x, const float y, const float z)
    {
        _rotation[0] = x;
        _rotation[1] = y;
        _rotation[2] = z;
    }

    void Planet::setScale(const float x, const float y, const float z)
    {
        _scale[0] = x;
        _scale[1] = y;
        _scale[2] = z;
    }

    void Planet::setOrbirRadius(const float radius)
    {
        _orbit_radius = radius;
    }

    void Planet::setAngle(const float angle)
    {
        _angle = angle;
    }

    void Planet::setOrbitSpeed(const float speed)
    {
        _orbit_speed = speed;
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

        _index_buffer = new IndexBuffer(Planet::getIndices(), Planet::getIndexSize());

        _vertex_array_object->addVertexBuffer(*_vertex_buffer_position);
        _vertex_array_object->addVertexBuffer(*_vertex_buffer_normal);
        _vertex_array_object->addVertexBuffer(*_vertex_buffer_texCoords);
        _vertex_array_object->setIndexBuffer(*_index_buffer);
    }
}
