#include "ShaderProgram.hpp"
#include "PositronEngineCore/Log.hpp"

#include <glad/glad.h>

namespace PositronEngine
{
    bool createShader(const char* shader_source, const GLenum shader_type, GLuint& shader_id)
    {
        shader_id = glCreateShader(shader_type);
        glShaderSource(shader_id, 1, &shader_source, nullptr);
        glCompileShader(shader_id);

        GLint success_compile;
        glGetShaderiv(shader_id,GL_COMPILE_STATUS ,&success_compile);

        if(success_compile == GL_FALSE)
        {
            char log_information[1024];
            glGetShaderInfoLog(shader_id, sizeof(log_information), nullptr, log_information);

            LOG_CRITICAL("SHADER is not compiled, error:\n{}", log_information);
            return false;
        }

        return true;
    }

    ShaderProgram::ShaderProgram(const char* vertex_shader_source, const char* fragment_shader_source)
    {
        GLuint vertex_shader_id = 0;
        if(!createShader(vertex_shader_source, GL_VERTEX_SHADER, vertex_shader_id))
        {
            LOG_CRITICAL("VERTEX SHADER IS NOT COMPILED !");
            glDeleteShader(vertex_shader_id);
            return;
        }

        GLuint fragment_shader_id = 0;
        if(!createShader(fragment_shader_source, GL_FRAGMENT_SHADER, fragment_shader_id))
        {
            LOG_CRITICAL("FRAGMENT SHADER IS NOT COMPILED !");
            glDeleteShader(vertex_shader_id);
            glDeleteShader(fragment_shader_id);
            return;
        }

        _id = glCreateProgram();
        glAttachShader(_id, vertex_shader_id);
        glAttachShader(_id, fragment_shader_id);

        glLinkProgram(_id);

        GLint success_program;
        glGetProgramiv(_id, GL_LINK_STATUS, &success_program);

        if(success_program == GL_FALSE)
        {
            char log_information[1024];

            glGetShaderInfoLog(_id, sizeof(log_information), nullptr, log_information);
            LOG_CRITICAL("SHADER PROGRAM is not compiled, error:\n{}", log_information);

            glDeleteProgram(_id);
            _id = 0;

            glDeleteShader(vertex_shader_id);
            glDeleteShader(fragment_shader_id);

            return;
        }
        else
        {
            _is_compiled = true;
        }

        //опенГЛ почему-то сам не удаляет шейдеры, делаем ручками (ТЕРПИМ) :^)
        glDetachShader(_id, vertex_shader_id);
        glDetachShader(_id, fragment_shader_id);

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(_id);
    }

    void ShaderProgram::bind() const
    {
        glUseProgram(_id);
    }

    void ShaderProgram::unbind()
    {
        glUseProgram(0);
    }

    ShaderProgram::ShaderProgram(ShaderProgram && shader_program)
    {
        _id = shader_program._id;
        _is_compiled = shader_program._is_compiled;

        shader_program._id = 0;
        shader_program._is_compiled = false;
    }

    ShaderProgram & ShaderProgram::operator=(ShaderProgram && shader_program)
    {
        glDeleteProgram(_id);
        _id = shader_program._id;
        _is_compiled = shader_program._is_compiled;

        shader_program._id = 0;
        shader_program._is_compiled = false;

        return *this;
    }
}
