#include "RenderOpenGL.hpp"
#include "VertexArray.hpp"
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
        glClear(GL_COLOR_BUFFER_BIT);
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

    const char* RenderOpenGL::getVersionOpenGL()
    {
        return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }
}
