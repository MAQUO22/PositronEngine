#include "PositronEngineCore/LightSources/PointLight.hpp"

namespace PositronEngine
{
    PointLight::PointLight()
    {
        LightTypeCounter::incrementPointLightCount();

    }

    PointLight::~PointLight()
    {
        LightTypeCounter::decrementPointLightCount();
    }

    float* PointLight::getLocation()
    {
        return _location;
    }

    glm::vec3 PointLight::getLocationVec3() const
    {
        return glm::vec3(_location[0], _location[1], _location[2]);
    }

    void PointLight::setLocation(const float x, const float y, const float z)
    {
        _location[0] = x;
        _location[1] = y;
        _location[2] = z;
    }

    void PointLight::setLocationVec3(const glm::vec3& location)
    {
        _location[0] = location.x;
        _location[1] = location.y;
        _location[2] = location.z;
    }

    void PointLight::setConstantAttenuation(const float attenuation)
    {
        _constantAttenuation = attenuation;
    }

    void PointLight::setLinearAttenuation(const float attenuation)
    {
        _linearAttenuation = attenuation;
    }

    float PointLight::getConstantAttenuation() const
    {
        return _constantAttenuation;
    }

    float PointLight::getLinearAttenuation() const
    {
        return _constantAttenuation;
    }

    void PointLight::draw(Camera& camera) {}
}
