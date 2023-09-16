#include <PositronEngineCore/Window.hpp>
#include <PositronEngineCore/Log.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PositronEngine{

static bool is_GLFW_initialized_ = false;

Window::Window(std::string title, unsigned int width, unsigned int height) :
    _title(std::move(title)), _width(width), _heigth(height)
{
    int result_code = initialization();
}

Window::~Window(){
    shutDown();
}

void Window::onUpdate(){

    glClearColor(1,1,0,0);
    glClear(GL_COLOR_BUFFER_BIT); // <--- need GL
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

int Window::initialization(){

    LOG_INFORMATION("Window '{0}' is created", _title);

    if(!is_GLFW_initialized_){
        if (!glfwInit()){
            LOG_CRITICAL("GLFW has not been initialized!");
            return -1;
        }
        is_GLFW_initialized_ = true;
    }

    _window = glfwCreateWindow(_width, _heigth, _title.c_str(), nullptr, nullptr);
    if (!_window)
    {
        LOG_CRITICAL("Window '{0}' has not been created!", _title);
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(_window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        LOG_CRITICAL("Initialization of GLAD is failed");
        return -3;
    }

    return 0;
}

void Window::shutDown(){
    glfwDestroyWindow(_window);
    glfwTerminate();
}

}
