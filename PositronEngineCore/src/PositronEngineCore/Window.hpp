#pragma once

#include <string>

struct GLFWwindow;

namespace PositronEngine{
class Window{
public:
    Window(std::string title, unsigned int width, unsigned int height);
    ~Window();

    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(const Window&&) = delete;

    virtual void onUpdate();

    unsigned int getWidth() const { return this->_width; }
    unsigned int getHeight() const { return this-> _heigth; }

private:
    int initialization();
    void shutDown();

    GLFWwindow* _window;
    std::string _title;
    unsigned int _width;
    unsigned int _heigth;

};
}

