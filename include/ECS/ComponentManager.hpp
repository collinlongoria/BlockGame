//
// Created by colli on 12/26/2024.
//

#ifndef COMPONENTMANAGER_HPP
#define COMPONENTMANAGER_HPP
#include <memory>

#include "Component.hpp"
#include "ComponentArray.hpp"
#include "Entity.hpp"
#include "Core/Log.hpp"


class ComponentManager {
public:
    template<typename T>
    void RegisterComponent() {
        const char* typeName = typeid(T).name();

        if(componentTypes.find(typeName) != componentTypes.end()) {
            Log::Output(Log::Severity::ERROR, "Registering component type more than once.");
            return;
        }

        // Add type to the map
        componentTypes.insert({typeName, nextComponentType});

        // Create a compoent array ptr and add it to the map
        componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

        // Increment to the next available register
        ++nextComponentType;
    }

    template<typename T>
    ComponentType GetComponentType() {
        const char* typeName = typeid(T).name();

        if(componentTypes.find(typeName) == componentTypes.end()) {
            Log::Output(Log::Severity::ERROR, "Component not registered before use.");
            return -1;
        }

        // Return component's type
        // Used for creating signatures
        return componentTypes[typeName];
    }

    template<typename T>
    void AddComponent(Entity entity, T component) {
        // Add a component to the array for an entity
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        // Remove a component from the array for an entity
        GetComponentArray<T>()->RemoveData(entity);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        // Return reference to a component from the array for an entity
        return GetComponentArray<T>()->GetData(entity);
    }

    void EntityDestroyed(Entity entity) {
        // Notify all arrays that an entity has been destroyed
        for(auto const& pair : componentArrays) {
            auto const& component = pair.second;
            component->EntityDestroyed(entity);
        }
    }

private:
    // Map from string pointer to component type
    std::unordered_map<const char*, ComponentType> componentTypes{};

    // Map from type string pointer to component array
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};

    // Component type to be assigned to the next registered component
    // Starts at 0
    ComponentType nextComponentType{};

    // Helper function to get statically casted pointer to the ComponentArray of type T
    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray() {
        const char* typeName = typeid(T).name();

        if(componentArrays.find(typeName) == componentArrays.end()) {
            Log::Output(Log::Severity::ERROR, "Component not registered.");
            return nullptr;
        }

        return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
    }
};



#endif //COMPONENTMANAGER_HPP
