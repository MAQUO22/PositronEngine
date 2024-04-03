#pragma once

#include "PositronEngineCore/LightObject.hpp"

namespace PositronEngine
{
    class DirectionLight : public LightObject
    {
        public:
            DirectionLight();
            ~DirectionLight() = default;

            void setDirection(const float x, const float y, const float z) override;
            void setDirectionVec3(const glm::vec3& direction) override;

            float* getDirection() override;
            glm::vec3 getDirectionVec3() const override;


            void draw(Camera& camera) override;
            void drawWithoutMesh(Camera& camera) override;

            LightType getLightType() override;
            glm::mat4 getSpaceMatrix() override;

        private:
            glm::mat4 calculateSpaceMatrix();

            float _direction[3] {1.0f, 0.0f, 1.0f};
            LightType _type;
    };
}
