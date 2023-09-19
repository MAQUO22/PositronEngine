#include <PositronEngineCore/Window.hpp>
#include <PositronEngineCore/Log.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PositronEngine{

static bool is_GLFW_initialized_ = false;

Window::Window(std::string title, unsigned int width, unsigned int height) :
    _window_data({std::move(title), width, height})
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

    LOG_INFORMATION("Window '{0}' is created", _window_data._title);

    if(!is_GLFW_initialized_){
        if (!glfwInit()){
            LOG_CRITICAL("GLFW has not been initialized!");
            return -1;
        }
        is_GLFW_initialized_ = true;
    }

    _window = glfwCreateWindow(_window_data._width, _window_data._heigth, _window_data._title.c_str(), nullptr, nullptr);
    if (!_window)
    {
        LOG_CRITICAL("Window '{0}' has not been created!", _window_data._title);
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(_window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        LOG_CRITICAL("Initialization of GLAD is failed");
        return -3;
    }

    glfwSetWindowUserPointer(_window, &_window_data);

    glfwSetWindowSizeCallback(_window,
        [](GLFWwindow* window,int width, int height)
        {
            WindowData& window_data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
            window_data._width = width;
            window_data._heigth = height;

            EventWindowResized event(width, height);
            window_data._event_callback_function(event);
        });

    glfwSetCursorPosCallback(_window,
        [](GLFWwindow* window, double x, double y)
        {
            WindowData& window_data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

            EventMouseMoved event(x,y);
            window_data._event_callback_function(event);
        }
    );

    glfwSetWindowCloseCallback(_window,
        [](GLFWwindow* window)
        {
             WindowData& window_data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

             EventWindowClose event;
             window_data._event_callback_function(event);
        }
    );

    return 0;
}

void Window::shutDown(){
    glfwDestroyWindow(_window);
    glfwTerminate();
}

}
