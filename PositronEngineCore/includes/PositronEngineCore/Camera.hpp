#pragma once
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace PositronEngine
{
    class Camera
    {
        public:
            enum class ProjectionMode
            {
                Perspective,
                Orthographic
            };

            Camera(const glm::vec3& location = {0.0f, 0.0f, 0.0f},
                   const glm::vec3& rotation = {0.0f, 0.0f, 0.0f},
                   const ProjectionMode projection_mode = ProjectionMode::Perspective);

            void setLocation(const glm::vec3& location);
            void setRotation(const glm::vec3& rotation);
            void setLocationAndRotation(const glm::vec3& location, const glm::vec3& rotation);
            void setProjection(const ProjectionMode projection_mode);

            glm::mat4 getViewMatrix();
            glm::vec3 getLocation() const { return _location; }
            glm::vec3 getRotation() const { return _rotation; }
            glm::mat4 getProjectionMatrix() const { return _projection_matrix; }

            void moveForward(const float delta);
            void moveUp(const float delta);
            void moveRight(const float delta);
            void moveAndRotation(const glm::vec3& move_delta, const glm::vec3& rotation_delta);

        private:
            void updateViewMatrix();
            void updateProjectionMatrix();

            glm::vec3 _location;
            glm::vec3 _rotation; // X - Roll, Y - Pitch, Z - Yaw
            glm::mat4 _view_matrix;

            glm::vec3 _direction;
            glm::vec3 _right;
            glm::vec3 _up;

            static constexpr glm::vec3 _world_forward{1.0f, 0.0f, 0.0f};
            static constexpr glm::vec3 _world_up{0.0f, 0.0f, 1.0f};
            static constexpr glm::vec3 _world_right{0.0f, -1.0f, 0.0f};

            glm::mat4 _projection_matrix;
            ProjectionMode _projection_mode;

            bool _update_view_matrix  = false;
    };
}
