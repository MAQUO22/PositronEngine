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

        private:
            static bool _keys_pressed[static_cast<int>(KeyCode::KEY_LAST)];
    };
}
