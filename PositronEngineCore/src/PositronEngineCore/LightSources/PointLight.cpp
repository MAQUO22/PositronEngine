#include "PositronEngineCore/LightSources/PointLight.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{
    glm::mat4 PointLight::calculateSpaceMatrix()
    {
        return glm::mat4(1.0f);
    }

    PointLight::PointLight(std::string name)
    {
        LightTypeCounter::incrementPointLightCount();
        _type = LightType::point;
        _name = name;

        _cube = std::make_shared<CubePrimitive>(_name);
        _cube->setScale(0.25f, 0.25f, 0.25f);
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

    float* PointLight::getPtrConstantAttenuation()
    {
        return &_constantAttenuation;
    }

    float PointLight::getConstantAttenuation()
    {
        return _constantAttenuation;
    }

    float* PointLight::getPtrLinearAttenuation()
    {
        return &_linearAttenuation;
    }

    float PointLight::getLinearAttenuation()
    {
        return _linearAttenuation;
    }

    void PointLight::setLightMaterial(const std::shared_ptr<LightMaterial>& light_material)
    {
        _light_material = light_material;
    }

    LightType PointLight::getLightType()
    {
        return _type;
    }

    glm::mat4 PointLight::getSpaceMatrix()
    {
        return calculateSpaceMatrix();
    }

    std::vector<glm::mat4> PointLight::getSpaceMatrices(float far_plane)
    {
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, far_plane);

        std::vector<glm::mat4> shadowTransforms;

        shadowTransforms.push_back(shadowProj * glm::lookAt(getLocationVec3(),
                                                            getLocationVec3() + glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0, 0.0, 0.0))); //гуд

        shadowTransforms.push_back(shadowProj * glm::lookAt(getLocationVec3(),
                                                            getLocationVec3() + glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 0.0, 0.0))); //гуд

        shadowTransforms.push_back(shadowProj * glm::lookAt(getLocationVec3(),
                                                            getLocationVec3() + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0))); //

        shadowTransforms.push_back(shadowProj * glm::lookAt(getLocationVec3(),
                                                            getLocationVec3() + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));

        shadowTransforms.push_back(shadowProj * glm::lookAt(getLocationVec3(),
                                                            getLocationVec3() + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0))); //гуд

        shadowTransforms.push_back(shadowProj * glm::lookAt(getLocationVec3(),
                                                            getLocationVec3() + glm::vec3(0.0, 0.0, -1.0), glm::vec3(1.0, 0.0, 0.0))); //гуд


        return shadowTransforms;
    }

    std::string PointLight::getName()
    {
        return _name;
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

            if(_light_material->getTexturesVector()[i].getType() == TextureType::diffuse)
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
