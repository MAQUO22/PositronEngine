#pragma once

#include "PositronEngineCore/GameObject.hpp"
#include "PositronEngineCore/Mesh.hpp"

namespace PositronEngine
{
    class CubePrimitive : public GameObject
    {
        public:
            CubePrimitive() = delete;
            CubePrimitive(std::string name);
            ~CubePrimitive();

            void draw() override;


        private:
            std::string _name;
            Mesh* _mesh = nullptr;

            //Material
            //Collision
            //и т.д.
    };
}
