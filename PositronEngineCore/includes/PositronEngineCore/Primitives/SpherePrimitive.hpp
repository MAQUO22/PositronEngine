#pragma once

#include "PositronEngineCore/GameObject.hpp"
#include "PositronEngineCore/Mesh.hpp"

namespace PositronEngine
{
    class SpherePrimitive : public GameObject
    {
        public:
            SpherePrimitive() = delete;
            SpherePrimitive(std::string name);
            ~SpherePrimitive();

            void draw() override;

        private:
            std::string _name;
            Mesh* _mesh = nullptr;
            //Material
            //Collision
            //и т.д.
    };
}

