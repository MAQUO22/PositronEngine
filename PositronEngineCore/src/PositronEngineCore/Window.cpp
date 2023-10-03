#include "PositronEngineCore/Window.hpp"
#include "PositronEngineCore/Log.hpp"

#include "PositronEngineCore/Rendering/OpenGL/RenderOpenGL.hpp"
#include "PositronEngineCore/Modules/GUImodule.hpp"

#include <GLFW/glfw3.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>


namespace PositronEngine
{


    Window::Window(std::string title, unsigned int width, unsigned int height) :
        _window_data({std::move(title), width, height})
    {
        initialization();
    }

    Window::~Window()
    {
        shutDown();
    }

    void Window::onUpdate()
    {
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    int Window::initialization()
    {
        LOG_INFORMATION("Window '{0}' is created", _window_data._title);

        if (!glfwInit())
        {
            LOG_CRITICAL("GLFW has not been initialized!");
            return -1;
        }

        _window = glfwCreateWindow(_window_data._width, _window_data._heigth, _window_data._title.c_str(), nullptr, nullptr);
        if (!_window)
        {
            LOG_CRITICAL("Window '{0}' has not been created!", _window_data._title);
            return -2;
        }

        if(!RenderOpenGL::initialize(_window))
        {
            return -3;
        }

        glfwSetWindowUserPointer(_window, &_window_data);

        glfwSetKeyCallback(_window,
            [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& window_data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

                switch(action)
                {
                    case GLFW_PRESS:
                    {
                        EventKeyPressed event(static_cast<KeyCode>(key), false);
                        window_data._event_callback_function(event);
                        break;
                    }

                    case GLFW_RELEASE:
                    {
                        EventKeyReleased event(static_cast<KeyCode>(key));
                        window_data._event_callback_function(event);
                        break;
                    }

                    case GLFW_REPEAT:
                    {
                        EventKeyPressed event(static_cast<KeyCode>(key), true);
                        window_data._event_callback_function(event);
                        break;
                    }
                }
            });

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

        glfwSetFramebufferSizeCallback(_window,
            [](GLFWwindow* window,int width, int height)
            {
                RenderOpenGL::setViewport(width, height);
            }
        );

        GUImodule::onWindowCreate(_window);

        return 0;
    }

    void Window::shutDown()
    {
        GUImodule::onWindowClose();
        glfwDestroyWindow(_window);
        glfwTerminate();

        LOG_INFORMATION("Window '{0}' is terminated.", _window_data._title);
    }
}
