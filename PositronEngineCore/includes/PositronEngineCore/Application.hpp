#pragma once

#include "PositronEngineCore/Event.hpp"
#include <memory>

namespace PositronEngine
{
    class Application
    {
        public:
            Application(const Application&) = delete;
            Application(Application&&) = delete;
            Application& operator=(const Application&) = delete;
            Application& operator=(const Application&&) = delete;

            Application();
            ~Application();

            virtual int start(unsigned int window_width,unsigned int window_height, const char* window_title);
            virtual void onUpdate() {}

        private:
            std::unique_ptr<class Window> _window;
            EventDispathcer _event_dispatcher;

            bool _is_window_alive = true;
    };
}

