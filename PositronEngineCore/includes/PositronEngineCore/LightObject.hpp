#pragma once

#include <glm/glm.hpp>

namespace PositronEngine
{
    class Camera;

    class LightObject
    {
        public:
            virtual void draw(Camera& camera) = 0;

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

            // virtual void setLocation(const glm::vec3& location) {}
            // virtual glm::vec3 getLocation() const { return glm::vec3(0.0f); }


        private:
            float _color[3] {1.0f, 1.0f, 1.0f};
            float _intensity {1.0f};
    };
}
