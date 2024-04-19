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

            void setFarPlane(const float far);
            void setNearPlane(const float near);
            void setViewportSize(const float width, const float height);
            void setFieldOfView(const float angle);

            glm::mat4 getViewMatrix();
            glm::vec3 getLocation() const { return _location; }
            glm::vec3 getRotation() const { return _rotation; }
            glm::vec3 getDirection() const { return _direction; }
            glm::vec3 getUp() const { return _up; }
            glm::mat4 getProjectionMatrix() const { return _projection_matrix; }

            const float getFarPlane() { return _far_plane; }
            const float getNearPlane() { return _near_plane; }
            const float getFieldOfView() { return _field_of_view; }
            const float getAspectRatio() { return _viewport_width/ _viewport_height; }

            void moveForward(const float delta);
            void moveUp(const float delta);
            void moveRight(const float delta);
            void moveAndRotation(const glm::vec3& move_delta, const glm::vec3& rotation_delta);

        private:
            void updateViewMatrix();
            void updateProjectionMatrix();

            float _far_plane { 100.0f };
            float _near_plane { 0.1f };
            float _viewport_width { 1920.0f };
            float _viewport_height { 1080.0f };

            float _field_of_view { 60.0f };

            glm::vec3 _location;
            glm::vec3 _rotation; // X - Roll, Y - Pitch, Z - Yaw
            glm::mat4 _view_matrix;

            glm::vec3 _direction;
            glm::vec3 _right;
            glm::vec3 _up;

            static glm::vec3 _world_forward;
            static glm::vec3 _world_up;
            static glm::vec3 _world_right;

            glm::mat4 _projection_matrix;
            ProjectionMode _projection_mode;

            bool _update_view_matrix  = false;
    };
}
