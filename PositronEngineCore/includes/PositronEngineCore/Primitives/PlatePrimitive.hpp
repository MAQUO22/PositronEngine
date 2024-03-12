#pragma once

#include "PositronEngineCore/GameObject.hpp"
#include "PositronEngineCore/Mesh.hpp"

namespace PositronEngine
{
    class PlatePrimitive : public GameObject
    {
        public:
            PlatePrimitive() = delete;
            PlatePrimitive(std::string name);
            ~PlatePrimitive();

            void draw() override;


        private:
            std::string _name;
            Mesh* _mesh = nullptr;

            //Material
            //Collision
            //и т.д.
    };
}

