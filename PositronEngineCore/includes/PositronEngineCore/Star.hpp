#pragma once
#include "PositronEngineCore/Sphere.h"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "PositronEngineCore/Texture2D.hpp"
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>


namespace PositronEngine
{
    class Star : public Sphere
    {
        public:
            Star(float radius, int sectors, int stacks, bool smooth, int up)
                : Sphere(radius, sectors, stacks, smooth, up)
                {}

            ~Star()
            {
                delete _vertex_array_object;
                delete _vertex_buffer_position;
                delete _vertex_buffer_normal;
                delete _vertex_buffer_texCoords;
                delete _index_buffer;
            }

            float* getLocation();
            float* getRotation();
            float* getScale();
            float* getLightColor();

            float getAmbientFactor();
            float getDiffuseFactor();
            float getOrbitRadius();
            float getOrbitSpeed();
            float getAngle();

            void setLocation(const float x, const float y, const float z);
            void setRotation(const float x, const float y, const float z);
            void setScale(const float x, const float y, const float z);
            void setLightColor(const float r, const float g, const float b);

            void setAmbientFactor(const float factor);
            void setDiffuseFactor(const float factor);
            void setOrbirRadius(const float radius);
            void setAngle(const float angle);
            void setOrbitSpeed(const float speed);

            void updateMatrix();
            void addRotation(const float vel);

            VertexArray* getVertexArrayObject();
            Texture2D* getTexture(const int id);
            glm::mat4 getModelMatrix();
            void setModelMatrix(glm::mat4 new_model_matrix);

            void setVertexArrayObject();
            void addTexture(const char* path);


        private:
            VertexArray* _vertex_array_object = nullptr;
            VertexBuffer* _vertex_buffer_position = nullptr;
            VertexBuffer* _vertex_buffer_normal = nullptr;
            VertexBuffer* _vertex_buffer_texCoords = nullptr;

            IndexBuffer* _index_buffer = nullptr;

            std::vector<Texture2D*> _textures_vector;

            BufferLayout _star_layout
            {
                ShaderDataType::Float3,
                ShaderDataType::Float3,
                ShaderDataType::Float2
            };

            float _location[3] = {0.0f, 0.0f, 0.0f};
            float _rotation[3] = {0.0f, 0.0f, 0.0f};
            float _scale[3] = {1.0f, 1.0f, 1.0f};

            float _light_color[3] = {1.0f, 1.0f, 1.0f};
            float _ambient_factor = 0.055f;
            float _diffuse_factor = 0.8f;

            float _orbit_radius = 0.0f;
            float _angle = 1.0f;
            float _orbit_speed = 0.0f;

            glm::mat4 _model_matrix;


    };
}
