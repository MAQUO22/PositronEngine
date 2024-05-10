#pragma once

#include <vector>

#include "PositronEngineCore/GameObject.hpp"
#include "PositronEngineCore/LightObject.hpp"

namespace PositronEngine
{
    class Scene
    {
        public:
            Scene(std::string name);
            Scene(const ShaderProgram&) = delete;
            Scene& operator= (const ShaderProgram&) = delete;
            ~Scene() = default;

            void addObject(std::unique_ptr<GameObject> object);
            void addLightObject(std::unique_ptr<LightObject> light_object);

            void removeObjectByName(const std::string& name);
            void removeLightObjectByName(const std::string& name);

            void removeObjectByIndex(size_t index);
            void removeLightObjectByIndex(size_t index);

            void setSelectedObjectIndex(int index);
            void setSelectedLightObjectIndex(int index);

            int getSelectedObjectIndex();
            int getSelectedLightObjectIndex();

            void loadScene(char* path);
            void saveScene();

            std::string getName();

            std::vector<std::unique_ptr<GameObject>>& getObjects();
            std::vector<std::unique_ptr<LightObject>>& getLightObjects();

            std::unique_ptr<GameObject>& getSelectedObject();
            std::unique_ptr<LightObject>& getSelectedLightObject();

        private:
            std::string _name;
            std::vector<std::unique_ptr<GameObject>> _objects;
            std::vector<std::unique_ptr<LightObject>> _light_objects;

            int _selected_object_index;
            int _selected_light_object_index;

            void removeObject(std::vector<std::unique_ptr<GameObject>>::iterator it);
            void removeLightObject(std::vector<std::unique_ptr<LightObject>>::iterator it);
    };
}
