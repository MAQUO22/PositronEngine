#include "PositronEngineCore/Camera.hpp"
#include <glm/trigonometric.hpp>

namespace PositronEngine
{
    Camera::Camera(const glm::vec3& location, const glm::vec3& rotation, const ProjectionMode projection_mode)
        : _location(location), _rotation(rotation), _projection_mode(projection_mode)
    {
        updateViewMatrix();
        updateProjectionMatrix();
    }


    void Camera::setLocation(const glm::vec3& location)
    {
        _location = location;
        updateViewMatrix();
    }
    void Camera::setRotation(const glm::vec3& rotation)
    {
        _rotation = rotation;
        updateViewMatrix();
    }
    void Camera::setLocationAndRotation(const glm::vec3& location, const glm::vec3& rotation)
    {
        _location = location;
        _rotation = rotation;
        updateViewMatrix();
    }

    void Camera::setProjection(const ProjectionMode projection_mode)
    {
        _projection_mode = projection_mode;
        updateProjectionMatrix();
    }

    void Camera::updateProjectionMatrix()
    {
        if(_projection_mode == ProjectionMode::Perspective)
        {
            float r = 0.1f;
            float t = 0.1f;
            float f = 10.0f;
            float n = 0.1f;

           _projection_matrix = glm::mat4(n/r,      0,          0,                       0,
                                          0,        n/t,        0,                       0,
                                          0,        0,          (-f-n) / (f-n),          -1,
                                          0,        0,          -2 * f * n / (f-n),      0);
        }
        else
        {
            float r = 2.0f;
            float t = 2.0f;
            float f = 100.0f;
            float n = 0.1f;

            _projection_matrix = glm::mat4(1/r,      0,          0,                       0,
                                           0,        1/t,        0,                       0,
                                           0,        0,          -2 / (f-n),              0,
                                           0,        0,          (-f-n) / (f-n),          1);
        }
    }

    void Camera::updateViewMatrix()
    {
        glm::mat4 location_matrix(1,                    0,                  0,                 0,
                                  0,                    1,                  0,                 0,
                                  0,                    0,                  1,                 0,
                                  -_location.x,        -_location.y,      -_location.z,     1);

        glm::mat4 rotate_matrix_x(1,    0,                                   0,                                   0,
                                  0,    cos(glm::radians(-_rotation.x)),     sin(glm::radians(-_rotation.x)),     0,
                                  0,    -sin(glm::radians(-_rotation.x)),    cos(glm::radians(-_rotation.x)),     0,
                                  0,    0,                                   0,                                  1);


        glm::mat4 rotate_matrix_y(cos(glm::radians(-_rotation.y)),       0,      -sin(glm::radians(-_rotation.y)),    0,
                                  0,                                     1,      0,                                   0,
                                  sin(glm::radians(-_rotation.y)),       0,      cos(glm::radians(-_rotation.y)),     0,
                                  0,                                     0,      0,                                   1);


        glm::mat4 rotate_matrix_z( cos(glm::radians(-_rotation.z)),      sin(glm::radians(-_rotation.z)),     0,      0,
                                 -sin(glm::radians(-_rotation.z)),       cos(glm::radians(-_rotation.z)),     0,      0,
                                 0,                                      0,                                   1,      0,
                                 0,                                      0,                                   0,      1);


        _view_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x * location_matrix;
    }
}


