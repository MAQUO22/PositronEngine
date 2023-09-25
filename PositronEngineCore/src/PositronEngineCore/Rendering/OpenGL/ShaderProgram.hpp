#pragma once

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

        private:
            bool _is_compiled = false;
            unsigned int _id = 0;
    };
}
