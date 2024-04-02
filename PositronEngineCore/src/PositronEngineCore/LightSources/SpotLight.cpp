#include "PositronEngineCore/LightSources/SpotLight.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{
    SpotLight::SpotLight(std::string name)
    {
        LightTypeCounter::incrementSpotLightCount();
        _type = LightType::spot;
        _name = name;

        _sphere = std::make_shared<SpherePrimitive>(_name);
        _sphere->setScale(0.5f, 0.5f, 0.5f);
    }

    SpotLight::~SpotLight()
    {
        LightTypeCounter::decrementSpotLightCount();
    }

    float* SpotLight::getLocation()
    {
        return _location;
    }

    glm::vec3 SpotLight::getLocationVec3() const
    {
        return glm::vec3(_location[0], _location[1], _location[2]);
    }

    void SpotLight::setLocation(const float x, const float y, const float z)
    {
        _location[0] = x;
        _location[1] = y;
        _location[2] = z;
    }

    void SpotLight::setLocationVec3(const glm::vec3& location)
    {
        _location[0] = location.x;
        _location[1] = location.y;
        _location[2] = location.z;
    }

    void SpotLight::setDirection(const float x, const float y, const float z)
    {
        _direction[0] = x;
        _direction[1] = y;
        _direction[2] = z;
    }

    void SpotLight::setDirectionVec3(const glm::vec3& direction)
    {
        _direction[0] = direction.x;
        _direction[1] = direction.y;
        _direction[2] = direction.z;
    }

    float* SpotLight::getDirection()
    {
        return _direction;
    }

    glm::vec3 SpotLight::getDirectionVec3() const
    {
        return glm::vec3(_direction[0], _direction[1], _direction[2]);
    }

    void SpotLight::setOuterCone(const float outer_cone)
    {
        _outer_cone = outer_cone;
    }

    float* SpotLight::getPtrOuterCone()
    {
        return &_outer_cone;
    }

    float SpotLight::getOuterCone()
    {
        return _outer_cone;
    }

    void SpotLight::setInnerCone(const float inner_cone)
    {
        _inner_cone = inner_cone;
    }

    float* SpotLight::getPtrInnerCone()
    {
        return &_inner_cone;
    }

    float SpotLight::getInnerCone()
    {
        return _inner_cone;
    }

    void SpotLight::setLightMaterial(const std::shared_ptr<LightMaterial>& light_material)
    {
        _light_material = light_material;
    }

    LightType SpotLight::getLightType()
    {
        return _type;
    }
    void SpotLight::draw(Camera& camera)
    {

        _sphere->setLocation(_location[0], _location[1], _location[2]);
        _sphere->updateModelMatrix();

        _light_material->getShaderProgram()->bind();
        _light_material->getShaderProgram()->setVec3("light_color", this->getColorVec3());
        _light_material->getShaderProgram()->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());

        _light_material->getShaderProgram()->setMatrix4("model_matrix", _sphere->getModelMatrix());

        _light_material->getShaderProgram()->setBool("textureAvailable", _light_material->getTexturesVector().size());

        for(size_t i = 0; i < _light_material->getTexturesVector().size(); i++)
        {

            if(_light_material->getTexturesVector()[i].getType() == TextureType::diffuse)
            {
                _light_material->getTexturesVector()[i].bindUnit(0);
            }
        }

        RenderOpenGL::draw(*_sphere->getMesh()->getVertexArray());

        for(size_t i = 0; i < _light_material->getTexturesVector().size(); i++)
            _light_material->getTexturesVector()[i].unbindUnit();

    }

    void SpotLight::drawWithoutMesh(Camera& camera) { }
}
