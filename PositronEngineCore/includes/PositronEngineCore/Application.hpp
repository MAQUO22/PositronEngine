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

            virtual void onInputUpdate() {}
            virtual void onGUIdraw() {}
            virtual void onMouseButtonEvent(const MouseButtonCode mouse_button, const double x, const double y, bool pressed) {}


            void computeHalfVertex(const float v1[3], const float v2[3], float newV[3]);

            glm::vec2 getCurrentCursorPosition() const;

            Camera camera { glm::vec3(-5, 0, 0) };

            float camera_location[3] = {0.0f, 0.0f, 3.0f};
            float camera_rotation[3] = {0.0f, 0.0f, 0.0f};

            float camera_speed = 0.035f;
            float camera_FOV = 60.0f;
            float camera_near_plane = 0.1f;
            float camera_far_plane = 100.0f;



            bool is_perspective_mode = true;

        private:
            std::unique_ptr<class Window> _window;
            EventDispathcer _event_dispatcher;

            bool _is_window_alive = true;
    };
}

