#include "PositronEngineCore/Star.hpp"

namespace PositronEngine
{
    float* PositronEngine::Star::getLocation()
    {
        return _location;
    }

    float* PositronEngine::Star::getRotation()
    {
        return _rotation;
    }

    float* PositronEngine::Star::getScale()
    {
        return _scale;
    }

    float* PositronEngine::Star::getLightColor()
    {
        return _light_color;
    }

    VertexArray* Star::getVertexArrayObject()
    {
        return _vertex_array_object;
    }

    Texture2D* Star::getTexture(const int id)
    {
        return _textures_vector[id];
    }

    glm::mat4 Star::getModelMatrix()
    {
        return _model_matrix;
    }

    float Star::getAmbientFactor()
    {
        return _ambient_factor;
    }

    float Star::getDiffuseFactor()
    {
        return _diffuse_factor;
    }

    float Star::getOrbitRadius()
    {
        return _orbit_radius;
    }

    float Star::getOrbitSpeed()
    {
        return _orbit_speed;
    }

    float Star::getAngle()
    {
        return _angle;
    }

    void Star::setModelMatrix(glm::mat4 new_model_matrix)
    {
        _model_matrix = new_model_matrix;
    }

    void Star::updateMatrix()
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

    void Star::addRotation(const float vel){
        _rotation[2] -= vel;
    }

    void Star::addTexture(const char* path)
    {;
        _textures_vector.push_back(new Texture2D(path));
    }

    void Star::setLocation(const float x, const float y, const float z)
    {
        _location[0] = x;
        _location[1] = y;
        _location[2] = z;
    }

    void Star::setRotation(const float x, const float y, const float z)
    {
        _rotation[0] = x;
        _rotation[1] = y;
        _rotation[2] = z;
    }

    void Star::setScale(const float x, const float y, const float z)
    {
        _scale[0] = x;
        _scale[1] = y;
        _scale[2] = z;
    }

    void Star::setLightColor(const float r, const float g, const float b)
    {
        _light_color[0] = r;
        _light_color[1] = g;
        _light_color[2] = b;
    }


    void Star::setAmbientFactor(const float factor)
    {
        _ambient_factor = factor;
    }

    void Star::setDiffuseFactor(const float factor)
    {
        _diffuse_factor = factor;
    }

    void Star::setOrbirRadius(const float radius)
    {
        _orbit_radius = radius;
    }

    void Star::setAngle(const float angle)
    {
        _angle = angle;
    }

    void Star::setOrbitSpeed(const float speed)
    {
        _orbit_speed = speed;
    }

    void PositronEngine::Star::setVertexArrayObject()
    {
        _vertex_array_object = new VertexArray();

        _vertex_buffer_position = new VertexBuffer(Star::getInterleavedVertices(),
                                                   Star::getInterleavedVertexSize(),
                                                   _star_layout);

        _vertex_buffer_normal = new VertexBuffer(Star::getNormals(),
                                                 Star::getNormalSize(),
                                                 _star_layout);

        _vertex_buffer_texCoords = new VertexBuffer(Star::getTexCoords(),
                                                    Star::getTexCoordSize(),
                                                    _star_layout);

        _index_buffer = new IndexBuffer(Star::getIndices(), Star::getIndexSize());

        _vertex_array_object->addVertexBuffer(*_vertex_buffer_position);
        _vertex_array_object->addVertexBuffer(*_vertex_buffer_normal);
        _vertex_array_object->addVertexBuffer(*_vertex_buffer_texCoords);
        _vertex_array_object->setIndexBuffer(*_index_buffer);
    }
}
