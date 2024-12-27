//
// Created by colli on 12/27/2024.
//

#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP
#include <functional>
#include <typeindex>
#include <unordered_map>

#include "Event.hpp"

class EventManager {
public:
    // Listener type
    using Listener = std::function<void(const Event&)>;

    // Adds a listener to a specific event type
    template<typename T>
    void AddListener(Listener listener) {
        auto& registeredListeners = listeners[typeid(T)];
        registeredListeners.emplace_back(std::move(listener));
    }

    // Dispatch an event to all listeners
    template<typename T>
    void Dispatch(const T& event) {
        auto it = listeners.find(typeid(T));
        if(it != listeners.end()) {
            for(const auto& listener : it->second) {
                listener(event);
            }
        }
    }
private:
    // Map of event type to list of listeners
    std::unordered_map<std::type_index, std::vector<Listener>> listeners;
};

#endif //EVENTMANAGER_HPP
