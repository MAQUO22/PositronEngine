#include "PositronEngineCore/LightSources/DirectionLight.hpp"
#include "PositronEngineCore/Camera.hpp"

namespace PositronEngine
{
    DirectionLight::DirectionLight()
    {
        _type = LightType::direction;
    }


    void DirectionLight::setDirection(const float x, const float y, const float z)
    {
        _direction[0] = x;
        _direction[1] = y;
        _direction[2] = z;
    }

    void DirectionLight::setDirectionVec3(const glm::vec3& direction)
    {
        _direction[0] = direction.x;
        _direction[1] = direction.y;
        _direction[2] = direction.z;
    }

    float* DirectionLight::getDirection()
    {
        return _direction;
    }

    glm::vec3 DirectionLight::getDirectionVec3() const
    {
        return glm::vec3(_direction[0], _direction[1], _direction[2]);
    }

    LightType DirectionLight::getLightType()
    {
        return _type;
    }

    void DirectionLight::draw(Camera& camera) {}

    void DirectionLight::drawWithoutMesh(Camera& camera) {}
}
