#include "PositronEngineCore/GameObject.hpp"
#include <string>
#include <glm/ext/matrix_transform.hpp>


namespace PositronEngine
{

    void GameObject::updateModelMatrix()
    {
        glm::mat4 location_matrix(1,                    0,                 0,               0,
                                  0,                    1,                 0,               0,
                                  0,                    0,                 1,               0,
                                  _location[0],         _location[1],      _location[2],    1);


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

                    glm::mat4x4 _scale_matrix(_scale[0],    0,              0,          0,
                                              0,            _scale[1],      0,          0,
                                              0,            0,              _scale[2],  0,
                                              0,            0,              0,          1);

                    _model_matrix = location_matrix * _rotate_matrix_x * _rotate_matrix_y * _rotate_matrix_z * _scale_matrix;
    };

    void GameObject::setLocation(const float x, const float y, const float z)
    {
        _location[0] = x;
        _location[1] = y;
        _location[2] = z;
    }

    void GameObject::setRotation(const float x, const float y, const float z)
    {
        _rotation[0] = x;
        _rotation[1] = y;
        _rotation[2] = z;
    }

    void GameObject::setScale(const float x, const float y, const float z)
    {
        _scale[0] = x;
        _scale[1] = y;
        _scale[2] = z;
    }

    void GameObject::setMaterial(const std::shared_ptr<Material>& material) {}

    std::shared_ptr<Material> GameObject::getMaterial() { return nullptr; }

    float* GameObject::getLocation()
    {
        return _location;
    }

    float* GameObject::getRotation()
    {
        return _rotation;
    }

    float* GameObject::getScale()
    {
        return _scale;
    }

    glm::vec3 GameObject::getLocationVec3()
    {
        return glm::vec3(_location[0], _location[1], _location[2]);
    }

    glm::vec3 GameObject::getRotationVec3()
    {
        return glm::vec3(_rotation[0], _rotation[1], _rotation[2]);
    }

    glm::vec3 GameObject::getScaleVec3()
    {
        return glm::vec3(_scale[0], _scale[1], _scale[2]);
    }

    glm::mat4 GameObject::getModelMatrix()
    {
        return _model_matrix;
    }

    std::string GameObject::getName()
    {
        std::string emp = "";
        return emp;
    }

    ObjectType GameObject::getObjectType()
    {
        return ObjectType::unknown;
    }

}
