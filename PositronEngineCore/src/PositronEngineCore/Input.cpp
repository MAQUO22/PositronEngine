#include "PositronEngineCore/Input.hpp"

namespace PositronEngine
{
    bool Input::_keys_pressed[static_cast<int>(KeyCode::KEY_LAST)] = {};

    bool Input::isKeyPressed(const KeyCode key_code)
    {
        return _keys_pressed[static_cast<int>(key_code)];
    }

    void Input::pressKey(const KeyCode key_code)
    {
        _keys_pressed[static_cast<int>(key_code)] = true;
    }

    void Input::releaseKey(const KeyCode key_code)
    {
        _keys_pressed[static_cast<int>(key_code)] = false;
    }
}
