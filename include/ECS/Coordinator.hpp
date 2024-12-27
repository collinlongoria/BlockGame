//
// Created by colli on 12/26/2024.
//

#ifndef COORDINATOR_HPP
#define COORDINATOR_HPP
#include "ComponentManager.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "SystemManager.hpp"


class Coordinator {
public:
    void Init() {
        // Create the managers
        componentManager = std::make_unique<ComponentManager>();
        entityManager = std::make_unique<EntityManager>();
        systemManager = std::make_unique<SystemManager>();
        eventManager = std::make_unique<EventManager>();
    }

    // Entity Functions:
    Entity CreateEntity() {
        return entityManager->CreateEntity();
    }

    void DestroyEntity(Entity entity) {
        entityManager->DestroyEntity(entity);

        componentManager->EntityDestroyed(entity);

        systemManager->EntityDestroyed(entity);
    }

    // Component Functions:
    template<typename T>
    void RegisterComponent() {
        componentManager->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component) {
        componentManager->AddComponent<T>(entity, component);

        auto signature = entityManager->GetSignature(entity);
        signature.set(componentManager->GetComponentType<T>(), true);
        entityManager->SetSignature(entity, signature);

        systemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        componentManager->RemoveComponent<T>(entity);

        auto signature = entityManager->GetSignature(entity);
        signature.set(componentManager->GetComponentType<T>(), false);
        entityManager->SetSignature(entity, signature);

        systemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return componentManager->GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType() {
        return componentManager->GetComponentType<T>();
    }

    // System Methods:
    template<typename T>
    std::shared_ptr<T> RegisterSystem() {
        return systemManager->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature signature) {
        systemManager->SetSignature<T>(signature);
    }

    // helper function to register event listeners
    // will this work? idk
    template <typename EventType, typename ListenerClass>
    void AddEventListener(ListenerClass* instance, void (ListenerClass::*listenerMethod)(const EventType&)) {
        eventManager->AddListener<EventType>(
            [instance, listenerMethod](const Event& e) {
                (instance->*listenerMethod)(static_cast<const EventType&>(e));
            });
    }

    // for free functions not owned by a class
    template <typename EventType>
    void AddEventListener(void (*listenerFunction)(const EventType&)) {
        eventManager->AddListener<EventType>([listenerFunction](const Event& e) {
            listenerFunction(static_cast<const EventType&>(e));
        });
    }

    template<typename T>
    void DispatchEvent(const T& event) {
        eventManager->Dispatch(event);
    }

private:
    std::unique_ptr<ComponentManager> componentManager;
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<SystemManager> systemManager;
    std::unique_ptr<EventManager> eventManager;
};



#endif //COORDINATOR_HPP
