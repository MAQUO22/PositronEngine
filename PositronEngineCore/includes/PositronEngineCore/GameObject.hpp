#pragma once

#include <glm/mat4x4.hpp>
#include <vector>
#include <memory>
#include <glm/trigonometric.hpp>

namespace PositronEngine
{
    class Camera;
    class LightObject;
    class Material;

    class GameObject
    {
        public:

            virtual void draw(Camera& camera, LightObject& direction_light, std::vector<std::unique_ptr<LightObject>>& point_lights) = 0;

            virtual ~GameObject() = default;

            void updateModelMatrix();

            void setLocation(const float x, const float y, const float z);
            void setRotation(const float x, const float y, const float z);
            void setScale(const float x, const float y, const float z);

            float* getLocation();
            float* getRotation();
            float* getScale();

            virtual std::string getName();

            glm::vec3 getLocationVec3();
            glm::vec3 getRotationVec3();
            glm::vec3 getScaleVec3();

            glm::mat4 getModelMatrix();

            virtual void setMaterial(Material* material);

        private:

            float _location[3] = {0.0f, 0.0f, 0.0f};
            float _rotation[3] = {0.0f, 0.0f, 0.0f};
            float _scale[3] = {1.0f, 1.0f, 1.0f};

            glm::mat4 _model_matrix;
    };
}
