#pragma once

#include <glm/mat4x4.hpp>

namespace PositronEngine
{
    class ShaderProgram
    {
        public:
            ShaderProgram() = delete;
            ShaderProgram(const ShaderProgram&) = delete;
            ShaderProgram& operator= (const ShaderProgram&) = delete;

            ShaderProgram(const char* vertex_shader_source, const char* fragment_shader_source);
            ~ShaderProgram();
            ShaderProgram(ShaderProgram&& shader_program);
            ShaderProgram& operator= (ShaderProgram&& shader_program);

            void bind() const;
            static void unbind();
            bool isCompile() const {return _is_compiled; }

            void setMatrix4(const char* matrix_name, const glm::mat4& matrix) const;
            void setInt(const char* name, const int value) const;
            void setFloat(const char* name, const float value) const;
            void setVec3(const char* name, const glm::vec3& value) const;
            void setVec2(const char* name, const glm::vec2& value) const;
        private:
            bool _is_compiled = false;
            unsigned int _id = 0;
    };
}
