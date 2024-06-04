#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/VertexArray.hpp"
#include "PositronEngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PositronEngine
{
    namespace Debug
    {
        const char* glSourceToString(const GLenum source)
        {
            switch(source)
            {
                case GL_DEBUG_SOURCE_API: return "GL_DEBUG_SOURCE_API";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
                case GL_DEBUG_SOURCE_SHADER_COMPILER: return "GL_DEBUG_SOURCE_SHADER_COMPILER";
                case GL_DEBUG_SOURCE_THIRD_PARTY: return "GL_DEBUG_SOURCE_THIRD_PARTY";
                case GL_DEBUG_SOURCE_APPLICATION: return "GL_DEBUG_SOURCE_APPLICATION";
                case GL_DEBUG_SOURCE_OTHER: return "GL_DEBUG_SOURCE_OTHER";

                default: return "UNKNOWN_DEBUG_SOURCE";
            }
        }

        const char* glTypeToString(const GLenum type)
        {
            switch(type)
            {
                case GL_DEBUG_TYPE_ERROR: return "GL_DEBUG_TYPE_ERROR";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
                case GL_DEBUG_TYPE_PORTABILITY: return "GL_DEBUG_TYPE_PORTABILITY";
                case GL_DEBUG_TYPE_PERFORMANCE: return "GL_DEBUG_TYPE_PERFORMANCE";
                case GL_DEBUG_TYPE_MARKER: return "GL_DEBUG_TYPE_MARKER";
                case GL_DEBUG_TYPE_PUSH_GROUP: return "GL_DEBUG_TYPE_PUSH_GROUP";
                case GL_DEBUG_TYPE_POP_GROUP: return "GL_DEBUG_TYPE_POP_GROUP";
                case GL_DEBUG_TYPE_OTHER: return "GL_DEBUG_TYPE_OTHER";

                default: return "UNKNOWN_DEBUG_TYPE";
            }
        }
    };

    bool RenderOpenGL::initialize(GLFWwindow* window)
    {
        glfwMakeContextCurrent(window);

        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            LOG_CRITICAL("GLAD IS FAILED :^(");
            return false;
        }

        LOG_INFORMATION("OpenGL context is initialized(ver : {0})", RenderOpenGL::getVersionOpenGL());

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);

        glDebugMessageCallback([](GLenum source, GLenum type,GLuint id,
                                  GLenum severity, GLsizei length,
                                  const GLchar *message, const void *userParam)
        {
            switch(severity)
            {
                case GL_DEBUG_SEVERITY_HIGH:
                    LOG_ERROR("OpenGL Error: [{0}:{1}]({2}): {3}",
                              Debug::glSourceToString(source),
                              Debug::glTypeToString(type),
                              id, message);
                    break;
                case GL_DEBUG_SEVERITY_MEDIUM:
                    LOG_WARNING("OpenGL Warning: [{0}:{1}]({2}: {3})",
                                Debug::glSourceToString(source),
                                Debug::glTypeToString(type),
                                id, message);
                    break;
                case GL_DEBUG_SEVERITY_LOW:
                    LOG_INFORMATION("OpenGL Information: [{0}:{1}]({2}: {3})",
                                    Debug::glSourceToString(source),
                                    Debug::glTypeToString(type),
                                    id, message);
                    break;
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    LOG_INFORMATION("OpenGL Notification: [{0}:{1}]({2}: {3})",
                                    Debug::glSourceToString(source),
                                    Debug::glTypeToString(type),
                                    id, message);
                    break;

                default:
                    LOG_CRITICAL("OpenGL Critical: [{0}:{1}]({2}: {3})",
                                    Debug::glSourceToString(source),
                                    Debug::glTypeToString(type),
                                    id, message);
            }
        }, nullptr);

        return true;
    }

    void RenderOpenGL::setBackgroundColor(const float* background_color_array)
    {
        glClearColor(background_color_array[0], background_color_array[1], background_color_array[2], background_color_array[3]);
    }

    void RenderOpenGL::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }
    void RenderOpenGL::setViewport(const unsigned int width, const unsigned int height,
                                    const unsigned int left_offset, const unsigned int bottom_offset)
    {
        glViewport(left_offset, bottom_offset, width, height);
    }

    void RenderOpenGL::draw(VertexArray& vertex_array)
    {
        vertex_array.bind();
        vertex_array.getVertexBuffer()->bind();
        vertex_array.getIndexBuffer()->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.getIndicesCount()), GL_UNSIGNED_INT, nullptr);
        vertex_array.unbind();
    }

    void RenderOpenGL::enableDepth()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void RenderOpenGL::disableDepth()
    {
        glDisable(GL_DEPTH_TEST);
    }

    const char* RenderOpenGL::getVersionOpenGL()
    {
        return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }

    double RenderOpenGL::getCurrentTime()
    {
        _current_time = glfwGetTime();
        return _current_time;
    }

    void RenderOpenGL::enableSync()
    {
        glfwSwapInterval(1);
    }

    void RenderOpenGL::disableSync()
    {
        glfwSwapInterval(0);
    }

    void RenderOpenGL::enableFaceCulling()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }

    void RenderOpenGL::disableFaceCulling()
    {
        glDisable(GL_CULL_FACE);
    }



    void RenderOpenGL::postFrame(double& frame_time)
    {
        _elapsed_time += frame_time;
        ++_frame_count;
        if(getElapsedTime() > 1.0f)
        {
            double fps = _frame_count / _elapsed_time;
            _elapsed_time = 0.0f;
            _frame_count = 0;
            //LOG_INFORMATION("FPS: {0}",fps);
        }
    }

}
