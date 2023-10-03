#pragma once

#include "KeyCode.hpp"

#include <array>
#include <functional>

namespace PositronEngine
{
    enum class EventType
    {
        WindowResized = 0,
        WindowClose,

        KeyPressed,
        KeyReleased,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,

        EventsCount
    };

    struct BaseEvent
    {
        virtual ~BaseEvent() = default;
        virtual EventType getType() const = 0;
    };

    struct EventWindowResized : public BaseEvent
    {
        unsigned int width;
        unsigned int height;
        static const EventType type = EventType::WindowResized;

        EventWindowResized(const unsigned int new_width, const unsigned int new_height)
            : width(new_width), height(new_height) { }

        virtual EventType getType() const override
        {
            return type;
        }
    };

    struct EventWindowClose : public BaseEvent
    {
        static const EventType type = EventType::WindowClose;

        virtual EventType getType() const override
        {
            return type;
        }
    };

    struct EventKeyPressed : public BaseEvent
    {
        KeyCode key_code;
        bool repeated;
        static const EventType type = EventType::KeyPressed;

        virtual EventType getType() const override
        {
            return type;
        }

        EventKeyPressed(const KeyCode _key_code, const bool _repeated)
            : key_code(_key_code), repeated(_repeated)
        {
        }
    };

    struct EventKeyReleased : public BaseEvent
    {
        KeyCode key_code;
        static const EventType type = EventType::KeyReleased;

        virtual EventType getType() const override
        {
            return type;
        }

        EventKeyReleased(const KeyCode _key_code)
            : key_code(_key_code)
        {
        }
    };

    struct EventMouseMoved : public BaseEvent
    {
        double x;
        double y;
        static const EventType type = EventType::MouseMoved;

        EventMouseMoved(const double new_x, const double new_y)
            : x(new_x), y(new_y) { }

        virtual EventType getType() const override
        {
            return type;
        }
    };

    class EventDispathcer
    {
        public:
            template<typename EventType>
            void addEventListener(std::function<void(EventType&)> callback)
            {
                auto base_callback = [function = std::move(callback)](BaseEvent& event)
                {
                    if(event.getType() == EventType::type)
                    {
                        function(static_cast<EventType&>(event));
                    }
                };

                _events_callbacks[static_cast<size_t>(EventType::type)] = std::move(base_callback);
            }

            void dispatch(BaseEvent& event)
            {
                auto& callback = _events_callbacks[static_cast<size_t>(event.getType())];

                if(callback)
                    callback(event);
            }

        private:
            std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> _events_callbacks;
    };
}
