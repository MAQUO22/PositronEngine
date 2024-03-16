#pragma once

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

namespace PositronEngine
{
    class Camera;

    class GameObject
    {
        public:

            virtual void draw(Camera& camera) = 0;

            virtual ~GameObject() {}

            void updateModelMatrix();

            void setLocation(const float x, const float y, const float z);
            void setRotation(const float x, const float y, const float z);
            void setScale(const float x, const float y, const float z);

            float* getLocation();
            float* getRotation();
            float* getScale();

            glm::vec3 getLocationVec3();
            glm::vec3 getRotationVec3();
            glm::vec3 getScaleVec3();

            glm::mat4 getModelMatrix();

        private:

            float _location[3] = {0.0f, 0.0f, 0.0f};
            float _rotation[3] = {0.0f, 0.0f, 0.0f};
            float _scale[3] = {1.0f, 1.0f, 1.0f};

            glm::mat4 _model_matrix;
    };
}
