#include "PositronEngineCore/LightSources/PointLight.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{
    PointLight::PointLight(std::string name)
    {
        LightTypeCounter::incrementPointLightCount();
        _name = name;
        _cube = std::make_shared<CubePrimitive>(_name);
        _cube->setScale(0.25f, 0.25f, 0.25f);
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

    void PointLight::setLightMaterial(const std::shared_ptr<LightMaterial>& light_material)
    {
        _light_material = light_material;
    }

    void PointLight::draw(Camera& camera)
    {

        _cube->setLocation(_location[0], _location[1], _location[2]);
        _cube->updateModelMatrix();

        _light_material->getShaderProgram()->bind();
        _light_material->getShaderProgram()->setVec3("light_color", this->getColorVec3());
        _light_material->getShaderProgram()->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());

        _light_material->getShaderProgram()->setMatrix4("model_matrix", _cube->getModelMatrix());

        _light_material->getShaderProgram()->setBool("textureAvailable", _light_material->getTexturesVector().size());

        for(size_t i = 0; i < _light_material->getTexturesVector().size(); i++)
        {

            if(_light_material->getTexturesVector()[i].getTextureType() == TextureType::diffuse)
            {
                _light_material->getTexturesVector()[i].bindUnit(0);
            }
        }

        RenderOpenGL::draw(*_cube->getMesh()->getVertexArray());

        for(size_t i = 0; i < _light_material->getTexturesVector().size(); i++)
            _light_material->getTexturesVector()[i].unbindUnit();

    }

    void PointLight::drawWithoutMesh(Camera& camera) { }
}
