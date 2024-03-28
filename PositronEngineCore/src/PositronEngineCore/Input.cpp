#include "PositronEngineCore/Input.hpp"

namespace PositronEngine
{
    bool Input::_keys_pressed[static_cast<int>(KeyCode::KEY_LAST) + 1] = {};
    bool Input::_mouse_buttons_pressed[static_cast<int>(MouseButtonCode::MOUSE_BUTTON_LAST) + 1] = {};

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

    bool Input::isMouseButtonPressed(const MouseButtonCode mouse_button)
    {
        return _mouse_buttons_pressed[static_cast<int>(mouse_button)];
    }

    void Input::releaseMouseButton(const MouseButtonCode mouse_button)
    {
        _mouse_buttons_pressed[static_cast<int>(mouse_button)] = false;
    }


    void Input::pressMouseButton(const MouseButtonCode mouse_button)
    {
        _mouse_buttons_pressed[static_cast<int>(mouse_button)] = true;
    }
}
