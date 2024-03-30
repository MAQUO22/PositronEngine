#pragma once

#include "PositronEngineCore/LightObject.hpp"
#include "PositronEngineCore/LightMaterial.hpp"
#include "PositronEngineCore/Primitives/SpherePrimitive.hpp"


namespace PositronEngine
{
    class SpotLight : public LightObject
    {
        public:
            SpotLight() = delete;
            SpotLight(std::string name);
            ~SpotLight();

            float* getLocation() override;
            glm::vec3 getLocationVec3() const override;

            void setLocation(const float x, const float y, const float z) override;
            void setLocationVec3(const glm::vec3& location) override;

            void setDirection(const float x, const float y, const float z) override;
            void setDirectionVec3(const glm::vec3& direction) override;

            float* getDirection() override;
            glm::vec3 getDirectionVec3() const override;

            void setOuterCone(const float outer_cone) override;
            float* getPtrOuterCone() override;
            float getOuterCone() override;

            void setInnerCone(const float inner_cone) override;
            float* getPtrInnerCone() override;
            float getInnerCone() override;

            void draw(Camera& camera) override;
            void drawWithoutMesh(Camera& camera) override;

            void setLightMaterial(const std::shared_ptr<LightMaterial>& light_material) override;

            LightType getLightType() override;

        private:
            float _location[3] {0.0f, 0.0f, 0.0f};
            float _direction[3] {1.0f, 0.0f, 0.0f};

            float _outer_cone {0.90f};
            float _inner_cone {0.95f};
            std::string _name;
            LightType _type;

            std::shared_ptr<LightMaterial> _light_material;
            std::shared_ptr<SpherePrimitive> _sphere;

    };
}
