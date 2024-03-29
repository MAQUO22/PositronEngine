#pragma once

#include "PositronEngineCore/LightObject.hpp"
#include "PositronEngineCore/LightMaterial.hpp"
#include "PositronEngineCore/Primitives/CubePrimitive.hpp"


namespace PositronEngine
{
    class PointLight : public LightObject
    {
        public:
            PointLight() = delete;
            PointLight(std::string name);
            ~PointLight() = default;

            float* getLocation() override;
            glm::vec3 getLocationVec3() const override;

            void setLocation(const float x, const float y, const float z) override;
            void setLocationVec3(const glm::vec3& location) override;

            void setConstantAttenuation(const float attenuation) override;
            void setLinearAttenuation(const float attenuation) override;

            float getConstantAttenuation() const override;
            float getLinearAttenuation() const override;

            void draw(Camera& camera) override;
            void drawWithoutMesh(Camera& camera) override;

            void setLightMaterial(const std::shared_ptr<LightMaterial>& light_material) override;

        private:
            float _location[3] {0.0f, 0.0f, 0.0f};
            float _constantAttenuation {0.05};
            float _linearAttenuation {0.01};
            std::string _name;
            std::shared_ptr<LightMaterial> _light_material;
            std::shared_ptr<CubePrimitive> _cube;
    };
}
