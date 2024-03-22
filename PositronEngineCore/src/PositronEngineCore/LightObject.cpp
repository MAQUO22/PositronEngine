#include "PositronEngineCore/LightObject.hpp"

namespace PositronEngine
{

    void LightObject::setColor(const float r, const float g, const float b)
    {
        _color[0] = r;
        _color[1] = g;
        _color[2] = b;
    }

    void LightObject::setIntensity(const float intensity)
    {
        _intensity = intensity;
    }

    float* LightObject::getColor()
    {
        return _color;
    }

    glm::vec3 LightObject::getColorVec3() const
    {
        return glm::vec3(_color[0], _color[1], _color[2]);
    }

    float LightObject::getIntensity() const
    {
        return _intensity;
    }

    void LightObject::setDirection(const float x, const float y, const float z)
    {
        // Пустая реализация метода
    }

    void LightObject::setDirectionVec3(const glm::vec3& direction) {}

    float* LightObject::getDirection()
    {
        return nullptr;
    }

    glm::vec3 LightObject::getDirectionVec3() const
    {
        return glm::vec3(0.0f);
    }

    void LightObject::draw(Camera& camera) {}

}
