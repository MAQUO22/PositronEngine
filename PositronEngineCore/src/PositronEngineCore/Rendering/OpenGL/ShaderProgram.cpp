#include "PositronEngineCore/ShaderProgram.hpp"
#include "PositronEngineCore/Log.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace PositronEngine
{
    const std::string PATH_TO_SHADERS = "../../ResourceFiles/Shaders/";

    std::string getShaderFromFile(const char* file_name)
    {
        std::ifstream in(file_name, std::ios::binary);
        if(in)
        {
            std::string shader;
            in.seekg(0, std::ios::end);
            shader.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&shader[0], shader.size());
            in.close();
            return(shader);
        }
        LOG_CRITICAL("SHADER IS NOT READ FROM THE FILE !");
        throw(errno);
    }


    bool createShader(const char* shader_file, const GLenum shader_type, GLuint& shader_id)
    {
        shader_id = glCreateShader(shader_type);
        std::string shader_code = getShaderFromFile((PATH_TO_SHADERS + std::string(shader_file)).c_str());
        const char* shader_source = shader_code.c_str();
        glShaderSource(shader_id, 1, &shader_source, nullptr);
        glCompileShader(shader_id);

        GLint success_compile;
        glGetShaderiv(shader_id,GL_COMPILE_STATUS ,&success_compile);

        if(success_compile == GL_FALSE)
        {
            char log_information[1024];
            glGetShaderInfoLog(shader_id, sizeof(log_information), nullptr, log_information);

            LOG_CRITICAL("Shader is not compiled, error:\n{0}", log_information);
            return false;
        }

        return true;
    }

    ShaderProgram::ShaderProgram(const char* vertex_shader_file, const char* fragment_shader_file)
    {
        GLuint vertex_shader_id = 0;
        if(!createShader(vertex_shader_file, GL_VERTEX_SHADER, vertex_shader_id))
        {
            LOG_CRITICAL("VERTEX SHADER IS NOT COMPILED !");
            glDeleteShader(vertex_shader_id);
            return;
        }

        GLuint fragment_shader_id = 0;
        if(!createShader(fragment_shader_file, GL_FRAGMENT_SHADER, fragment_shader_id))
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

        //опенГЛ почему-то сам не удаляет шейдеры, делаем ручками =ТЕРПИМ= :^)
        glDetachShader(_id, vertex_shader_id);
        glDetachShader(_id, fragment_shader_id);
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
    }

     ShaderProgram::ShaderProgram(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file)
    {
        GLuint vertex_shader_id = 0;
        if(!createShader(vertex_shader_file, GL_VERTEX_SHADER, vertex_shader_id))
        {
            LOG_CRITICAL("VERTEX SHADER IS NOT COMPILED !");
            glDeleteShader(vertex_shader_id);
            return;
        }

        GLuint fragment_shader_id = 0;
        if(!createShader(fragment_shader_file, GL_FRAGMENT_SHADER, fragment_shader_id))
        {
            LOG_CRITICAL("FRAGMENT SHADER IS NOT COMPILED !");
            glDeleteShader(vertex_shader_id);
            glDeleteShader(fragment_shader_id);
            return;
        }

        GLuint geometry_shader_id = 0;
        if(!createShader(geometry_shader_file, GL_GEOMETRY_SHADER, geometry_shader_id))
        {
            LOG_CRITICAL("GEOMETRY SHADER IS NOT COMPILED !");
            glDeleteShader(vertex_shader_id);
            glDeleteShader(fragment_shader_id);
            glDeleteShader(geometry_shader_id);
            return;
        }


        _id = glCreateProgram();
        glAttachShader(_id, vertex_shader_id);
        glAttachShader(_id, fragment_shader_id);
        glAttachShader(_id, geometry_shader_id);

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
            glDeleteShader(geometry_shader_id);

            return;
        }
        else
        {
            _is_compiled = true;

        }

        //опенГЛ почему-то сам не удаляет шейдеры, делаем ручками =ТЕРПИМ= :^)
        glDetachShader(_id, vertex_shader_id);
        glDetachShader(_id, fragment_shader_id);
        glDetachShader(_id, geometry_shader_id);

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        glDeleteShader(geometry_shader_id);
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

    void ShaderProgram::setMatrix4(const char* matrix_name, const glm::mat4& matrix) const
    {
        glUniformMatrix4fv(glGetUniformLocation(_id, matrix_name), 1, GL_FALSE,glm::value_ptr(matrix));
    }

    void ShaderProgram::setInt(const char* name, const int value) const
    {
        glUniform1i(glGetUniformLocation(_id, name), value);
    }

    void ShaderProgram::setVec3(const char* name, const glm::vec3& value) const
    {
        glUniform3f(glGetUniformLocation(_id, name), value.x, value.y, value.z);
    }

    void ShaderProgram::setVec2(const char* name, const glm::vec2& value) const
    {
        glUniform2f(glGetUniformLocation(_id, name), value.x, value.y);
    }

    void PositronEngine::ShaderProgram::setFloat(const char* name, const float value) const
    {
        glUniform1f(glGetUniformLocation(_id, name), value);

    }

    void PositronEngine::ShaderProgram::setBool(const char* name, const int value) const
    {
        if(value > 0){
            glUniform1i(glGetUniformLocation(_id, name), 1);
        }
        else
        {
            glUniform1i(glGetUniformLocation(_id, name), 0);
        }
    }

    void ShaderProgram::setDouble(const char* name, const double value) const
    {
        glUniform1d(glGetUniformLocation(_id, name), value);
    }

    unsigned int ShaderProgram::getID() const
    {
        return _id;
    }

}
