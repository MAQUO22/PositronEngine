#include "PositronEngineCore/Scene.hpp"
#include <algorithm>

namespace PositronEngine
{
    Scene::Scene(std::string name) : _name(name)
    {

    }

    void Scene::addObject(std::unique_ptr<GameObject> object)
    {
        _objects.emplace_back(std::move(object));
    }

    void Scene::addLightObject(std::unique_ptr<LightObject> light_object)
    {
        _light_objects.emplace_back(std::move(light_object));
    }

    void Scene::removeObjectByName(const std::string& name)
    {
        _objects.erase(std::remove_if(_objects.begin(), _objects.end(),
        [&name](const std::unique_ptr<GameObject>& obj)
        {
            return obj->getName() == name;
        }),
        _objects.end());
    }

    void Scene::removeLightObjectByName(const std::string& name)
    {
        _light_objects.erase(std::remove_if(_light_objects.begin(), _light_objects.end(),
        [&name](const std::unique_ptr<LightObject>& light_obj)
        {
            return light_obj->getName() == name;
        }),
        _light_objects.end());
    }

    void Scene::removeObjectByIndex(size_t index)
    {
        if (index < _objects.size())
        {
            auto it = _objects.begin() + index;
            removeObject(it);
        }
    }

    void Scene::removeLightObjectByIndex(size_t index)
    {
        if (index < _light_objects.size())
        {
            auto it = _light_objects.begin() + index;
            removeLightObject(it);
        }
    }

    void Scene::removeObject(std::vector<std::unique_ptr<GameObject>>::iterator it)
    {
        _objects.erase(it);
    }

    void Scene::removeLightObject(std::vector<std::unique_ptr<LightObject>>::iterator it)
    {
        _light_objects.erase(it);
    }

    std::string Scene::getName()
    {
        return _name;
    }
    std::vector<std::unique_ptr<GameObject>>& Scene::getObjects()
    {
        return _objects;
    }

    std::vector<std::unique_ptr<LightObject>>& Scene::getLightObjects()
    {
        return _light_objects;
    }


    void Scene::loadScene(char* path) {}
    void Scene::saveScene() {}
}
