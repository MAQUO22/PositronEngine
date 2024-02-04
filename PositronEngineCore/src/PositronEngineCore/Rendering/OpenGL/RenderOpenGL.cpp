#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/VertexArray.hpp"
#include "PositronEngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PositronEngine
{
    bool RenderOpenGL::initialize(GLFWwindow* window)
    {
        glfwMakeContextCurrent(window);

        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            LOG_CRITICAL("GLAD IS FAILED :^(");
            return false;
        }

        LOG_INFORMATION("OpenGL context is initialized(ver : {0})", RenderOpenGL::getVersionOpenGL());
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

    void RenderOpenGL::draw(const VertexArray& vertex_array)
    {
        vertex_array.bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.getIndicesCount()), GL_UNSIGNED_INT, nullptr);
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
            LOG_INFORMATION("FPS: {0}",fps);
        }
    }

}
