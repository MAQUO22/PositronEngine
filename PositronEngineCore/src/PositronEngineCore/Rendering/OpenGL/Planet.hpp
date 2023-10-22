#pragma once
#include "Sphere.h"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Texture2D.hpp"

namespace PositronEngine
{
    class Planet : public Sphere
    {
        public:
            Planet(float radius, int sectors, int stacks, bool smooth, int up)
                : Sphere(radius, sectors, stacks, smooth, up)
            {}

            ~Planet()
            {
                delete _vertex_array_object;
                delete _vertex_buffer_position;
                delete _vertex_buffer_normal;
                delete _vertex_buffer_texCoords;
                delete _index_buffer;
                delete _texture;
            }

            float* getLocation();
            float* getRotation();
            float* getScale();

            VertexArray* getVertexArrayObject();
            Texture2D* getTexture();

            void setVertexArrayObject();
            void setTexture(const char* path);

        private:
            VertexArray* _vertex_array_object = nullptr;
            VertexBuffer* _vertex_buffer_position = nullptr;
            VertexBuffer* _vertex_buffer_normal = nullptr;
            VertexBuffer* _vertex_buffer_texCoords = nullptr;

            IndexBuffer* _index_buffer = nullptr;

            Texture2D* _texture = nullptr;

            BufferLayout _planet_layout
            {
            ShaderDataType::Float3,
            ShaderDataType::Float3,
            ShaderDataType::Float2
            };


            float _location[3] = {0.0f, 0.0f, 0.0f};
            float _rotation[3] = {0.0f, 0.0f, 0.0f};
            float _scale[3] = {1.0f, 1.0f, 1.0f};
    };
}
