#pragma once

#include "KeyCode.hpp"

namespace PositronEngine
{
    class Input
    {
        public:
            static bool isKeyPressed(const KeyCode key_code);
            static void releaseKey(const KeyCode key_code);
            static void pressKey(const KeyCode key_code);

            static bool isMouseButtonPressed(const MouseButtonCode mouse_button);
            static void releaseMouseButton(const MouseButtonCode mouse_button);
            static void pressMouseButton(const MouseButtonCode mouse_button);

        private:
            static bool _keys_pressed[];
            static bool _mouse_buttons_pressed[];
    };
}
