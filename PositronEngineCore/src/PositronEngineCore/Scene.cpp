#include "PositronEngineCore/Scene.hpp"
#include "PositronEngineCore/Log.hpp"
#include <algorithm>

namespace PositronEngine
{
    Scene::Scene(std::string name) : _name(name) , _selected_object_index(-1), _selected_light_object_index(-1)
    {

    }

    void Scene::addObject(std::unique_ptr<GameObject> object)
    {
        if(isObjectNameAlreadyExists(object->getName()))
            LOG_CRITICAL("OBJECT WITH THIS NAME ALREADY EXISTS!");
        else
            _objects.emplace_back(std::move(object));
    }

    void Scene::addLightObject(std::unique_ptr<LightObject> light_object)
    {
        if(isLightObjectNameAlreadyExists(light_object->getName()))
            LOG_CRITICAL("LIGHT OBJECT WITH THIS NAME ALREADY EXISTS!");
        else
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

    bool Scene::isObjectNameAlreadyExists(std::string name)
    {
        for (const auto& obj : _objects)
        {
            if (obj->getName() == name) {
                return true;
            }
        }
        return false;
    }

    bool Scene::isLightObjectNameAlreadyExists(std::string name)
    {
        for (const auto& obj : _light_objects)
        {
            if (obj->getName() == name) {
                return true;
            }
        }
        return false;
    }

    void Scene::removeObject(std::vector<std::unique_ptr<GameObject>>::iterator it)
    {
        _objects.erase(it);
    }

    void Scene::removeLightObject(std::vector<std::unique_ptr<LightObject>>::iterator it)
    {
        _light_objects.erase(it);
    }

    void Scene::setSelectedObjectIndex(int index)
    {
        _selected_object_index = index;
    }

    void Scene::setSelectedLightObjectIndex(int index)
    {
        _selected_light_object_index = index;
    }

    int Scene::getSelectedObjectIndex()
    {
        return _selected_object_index;
    }

    int Scene::getSelectedLightObjectIndex()
    {
        return _selected_light_object_index;
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

    std::unique_ptr<GameObject>& Scene::getSelectedObject()
    {
        if(_selected_object_index >= 0 && _selected_object_index < _objects.size())
            return _objects[_selected_object_index];
        else
        {
            static std::unique_ptr<GameObject> null_ptr;
            return null_ptr;
        }

    }

    std::unique_ptr<LightObject>& Scene::getSelectedLightObject()
    {
        if(_selected_light_object_index >= 0 && _selected_light_object_index < _light_objects.size())
            return _light_objects[_selected_light_object_index];
        else
        {
            static std::unique_ptr<LightObject> null_ptr;
            return null_ptr;
        }
    }

    void Scene::loadScene(char* path) {}
    void Scene::saveScene() {}
}
