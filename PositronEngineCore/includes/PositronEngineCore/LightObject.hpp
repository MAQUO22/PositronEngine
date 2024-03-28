#pragma once

#include <glm/glm.hpp>
#include "PositronEngineCore/LightTypeCounter.hpp"

namespace PositronEngine
{
    class Camera;
    class LightMaterial;

    class LightObject
    {
        public:
            virtual void draw(Camera& camera) = 0;
            virtual void drawWithoutMesh(Camera& camera) = 0;

            virtual ~LightObject() = default;

            void setColor(const float r, const float g, const float b);
            void setIntensity(const float intensity);

            float* getColor();
            glm::vec3 getColorVec3() const;

            float getIntensity() const;

            virtual void setDirection(const float x, const float y, const float z);
            virtual void setDirectionVec3(const glm::vec3& direction);

            virtual float* getDirection();
            virtual glm::vec3 getDirectionVec3() const;

            virtual float* getLocation();
            virtual glm::vec3 getLocationVec3() const;

            virtual void setLocation(const float x, const float y, const float z);
            virtual void setLocationVec3(const glm::vec3& location);

            virtual void setConstantAttenuation(const float attenuation);
            virtual void setLinearAttenuation(const float attenuation);

            virtual float getConstantAttenuation() const;
            virtual float getLinearAttenuation() const;

            virtual void setLightMaterial(LightMaterial* light_material);

        private:
            float _color[3] {1.0f, 1.0f, 1.0f};
            float _intensity {1.0f};
    };
}
