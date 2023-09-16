#include "PositronEngineCore/Application.hpp"
#include <PositronEngineCore/Log.hpp>
#include <PositronEngineCore/Window.hpp>

#include <iostream>

namespace PositronEngine{
    Application::Application(){
        LOG_INFORMATION("Stating application");
    }

    Application::~Application(){
        LOG_INFORMATION("Closing application");
    }

    int Application::start(unsigned int window_width,unsigned int window_height, const char* window_title){
        _window = std::make_unique<Window>(window_title, window_width, window_height);

        while(true){
            _window->onUpdate();
            onUpdate();
        }
    }
}
