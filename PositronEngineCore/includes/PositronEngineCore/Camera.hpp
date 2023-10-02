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

            glm::mat4 getViewMatrix() const { return _view_matrix; }
            glm::mat4 getProjectionMatrix() const { return _projection_matrix; }

        private:
            void updateViewMatrix();
            void updateProjectionMatrix();

            glm::vec3 _location;
            glm::vec3 _rotation;
            glm::mat4 _view_matrix;
            glm::mat4 _projection_matrix;
            ProjectionMode _projection_mode;
    };
}
