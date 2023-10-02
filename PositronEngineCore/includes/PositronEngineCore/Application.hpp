#pragma once

#include "PositronEngineCore/Event.hpp"
#include "PositronEngineCore/Camera.hpp"
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
            virtual void onGUIdraw() {}

            Camera camera;
            float camera_location[3] = {0.0f, 0.0f, 1.0f};
            float camera_rotation[3] = {0.0f, 0.0f, 0.0f};

            bool is_perspective_mode = false;

        private:
            std::unique_ptr<class Window> _window;
            EventDispathcer _event_dispatcher;

            bool _is_window_alive = true;
    };
}

