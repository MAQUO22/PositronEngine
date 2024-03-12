#include "PositronEngineCore/Camera.hpp"

#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
        _update_view_matrix = true;
    }
    void Camera::setRotation(const glm::vec3& rotation)
    {
        _rotation = rotation;
        _update_view_matrix = true;
    }
    void Camera::setLocationAndRotation(const glm::vec3& location, const glm::vec3& rotation)
    {
        _location = location;
        _rotation = rotation;
        _update_view_matrix = true;
    }

    void Camera::setProjection(const ProjectionMode projection_mode)
    {
        _projection_mode = projection_mode;
        updateProjectionMatrix();
    }

    void Camera::setFarPlane(const float far)
    {
        _far_plane = far;
        updateProjectionMatrix();
    }

    void Camera::setNearPlane(const float near)
    {
        _near_plane = near;
        updateProjectionMatrix();
    }

    void Camera::setViewportSize(const float width , const float height)
    {
        _viewport_width = width;
        _viewport_height = height;
        updateProjectionMatrix();
    }

    void Camera::setFieldOfView(const float angle)
    {
        _field_of_view = angle;
        updateProjectionMatrix();
    }

    glm::mat4 Camera::getViewMatrix()
    {
        if(_update_view_matrix)
        {
            updateViewMatrix();
        }

        return _view_matrix;
    }

    void Camera::updateProjectionMatrix()
    {
        if(_projection_mode == ProjectionMode::Perspective)
        {
           _projection_matrix = glm::perspective(glm::radians(_field_of_view), _viewport_width / _viewport_height, _near_plane, _far_plane);
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

        const float roll_radians = glm::radians(_rotation.x);
        const float pitch_radians = glm::radians(_rotation.y);
        const float yaw_radians = glm::radians(_rotation.z);


        const glm::mat3 rotate_matrix_x(1,          0,                                   0,
                                        0,          cos(roll_radians),     sin(roll_radians),
                                        0,          -sin(roll_radians),    cos(roll_radians));


        const glm::mat3 rotate_matrix_y(cos(pitch_radians),       0,      -sin(pitch_radians),
                                        0,                        1,      0,
                                        sin(pitch_radians),       0,      cos(pitch_radians));


        const glm::mat3 rotate_matrix_z(cos(yaw_radians),        sin(yaw_radians),     0,
                                        -sin(yaw_radians),       cos(yaw_radians),     0,
                                        0,                       0,                    1);

        const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;

        _direction = glm::normalize(euler_rotate_matrix * _world_forward);
        _right = glm::normalize(euler_rotate_matrix * _world_right);
        _up = glm::cross(_right, _direction);

        _view_matrix = glm::lookAt(_location, _location + _direction, _up);
    }

    void Camera::moveForward(const float delta)
    {
        _location += _direction * delta;
        _update_view_matrix = true;
    }

    void Camera::moveUp(const float delta)
    {
        _location += _world_up * delta;
        _update_view_matrix = true;
    }

    void Camera::moveRight(const float delta)
    {
        _location += _right * delta;
        _update_view_matrix = true;
    }

    void Camera::moveAndRotation(const glm::vec3& move_delta, const glm::vec3& rotation_delta)
    {
        _location += _direction * move_delta.x;
        _location += _right * move_delta.y;
        _location += _up * move_delta.z;

        _rotation += rotation_delta;
        _update_view_matrix = true;
    }
}


