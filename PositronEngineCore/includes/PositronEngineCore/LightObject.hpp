#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <memory>
#include "PositronEngineCore/LightTypeCounter.hpp"

namespace PositronEngine
{
    class Camera;
    class LightMaterial;

    enum class LightType
    {
        direction,
        point,
        spot,
        unknown
    };

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

            virtual float* getPtrConstantAttenuation();
            virtual float* getPtrLinearAttenuation();

            virtual float getConstantAttenuation();
            virtual float getLinearAttenuation();

            virtual void setOuterCone(const float outer_cone);
            virtual float* getPtrOuterCone();
            virtual float getOuterCone();

            virtual void setInnerCone(const float inner_cone);
            virtual float* getPtrInnerCone();
            virtual float getInnerCone();

            virtual void setLightMaterial(const std::shared_ptr<LightMaterial>& light_material);

            virtual LightType getLightType() ;
            virtual glm::mat4 getSpaceMatrix();

        private:
            float _color[3] {1.0f, 1.0f, 1.0f};
            float _intensity {1.0f};
    };
}
