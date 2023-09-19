#pragma once

#include <string>
#include <functional>
#include "PositronEngineCore/Event.hpp"

struct GLFWwindow;

namespace PositronEngine{

class Window{
public:
    using eventCallbackFunction = std::function<void(BaseEvent&)>;

    Window(std::string title, unsigned int width, unsigned int height);
    ~Window();

    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(const Window&&) = delete;

    virtual void onUpdate();

    unsigned int getWidth() const { return this->_window_data._width; }
    unsigned int getHeight() const { return this->_window_data._heigth; }

    void setEventCallback(const eventCallbackFunction& callback)
    {
        _window_data._event_callback_function = callback;
    }

private:
    struct WindowData
    {
        std::string _title;
        unsigned int _width;
        unsigned int _heigth;
        eventCallbackFunction _event_callback_function;
    };

    int initialization();
    void shutDown();

    GLFWwindow* _window = nullptr;
    WindowData _window_data;

};
}

