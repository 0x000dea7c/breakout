#pragma once

#include "pch.hpp"

#include <unordered_map>
#include <functional>
#include <vector>

namespace Event {

    enum class EventType {
        MenuPlayGame,
        MenuQuitGame,
        MenuIncreaseVolume,
        MenuDecreaseVolume
    };

    // blehhhhhh, not rly needed now, but maybe in the future
    class Event final {
    public:
        explicit Event(EventType type)
            :eventType{ type }
        {
        }

        inline EventType getType() const { return eventType; }

    private:
        EventType eventType;
    };

    class EventManager final {
    public:
        inline void subscribe(EventType type, std::function<void(const Event&)> listener)
        {
            listeners[type].push_back(listener);
        }

        inline void post(const Event& event)
        {
            auto& eventListeners = listeners[event.getType()];
            for(auto& listener : eventListeners) {
                listener(event);
            }
        }

    private:
        std::unordered_map<EventType, std::vector<std::function<void(const Event&)>>> listeners;
    };
};
