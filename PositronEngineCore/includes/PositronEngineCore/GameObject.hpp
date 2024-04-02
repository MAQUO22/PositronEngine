#pragma once

#include <glm/mat4x4.hpp>
#include <vector>
#include <memory>
#include <glm/trigonometric.hpp>
#include <string>

namespace PositronEngine
{
    class Camera;
    class LightObject;
    class Material;
    class ShaderProgram;

    class GameObject
    {
        public:

            virtual void draw(Camera& camera,std::vector<std::unique_ptr<LightObject>>& light_sources) = 0;
            virtual void draw(std::shared_ptr<ShaderProgram>& shader_program, glm::mat4 space_matrix) = 0;

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

            virtual void setMaterial(const std::shared_ptr<Material>& material);
            virtual std::shared_ptr<Material> getMaterial();

        private:

            float _location[3] = {0.0f, 0.0f, 0.0f};
            float _rotation[3] = {0.0f, 0.0f, 0.0f};
            float _scale[3] = {1.0f, 1.0f, 1.0f};

            glm::mat4 _model_matrix;
    };
}
