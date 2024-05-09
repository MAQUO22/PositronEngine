#include "PositronEngineCore/LightSources/DirectionLight.hpp"
#include "PositronEngineCore/Camera.hpp"


namespace PositronEngine
{
    glm::mat4 DirectionLight::calculateSpaceMatrix()
    {
        glm::vec3 lightPosition = 10.0f * getDirectionVec3();

        glm::mat4 viewMatrix = glm::lookAt(lightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projectionMatrix = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, 0.1f, 200.0f);

        glm::mat4 lightSpaceMatrix = projectionMatrix * viewMatrix;

        return lightSpaceMatrix;
    }

    DirectionLight::DirectionLight(std::string name)
    {
        _name = name;
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

    glm::mat4 DirectionLight::getSpaceMatrix()
    {
        return calculateSpaceMatrix();
    }

    std::string DirectionLight::getName()
    {
        return _name;
    }

    void DirectionLight::draw(Camera& camera) {}

    void DirectionLight::drawWithoutMesh(Camera& camera) {}
}
