//
// Created by colli on 12/29/2024.
//

#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <memory>
#include <typeindex>

#include "ECS/Coordinator.hpp"
#include "ECS/System.hpp"
#include "ECS/Systems/CameraSystem.hpp"
#include "ECS/Systems/PhysicsSystem.hpp"
#include "ECS/Systems/RenderSystem.hpp"
#include "ECS/Systems/TransformSystem.hpp"
#include "Graphics/MeshManager.hpp"
#include "Graphics/ShaderManager.hpp"

struct Transform;
struct RigidBody;
struct Renderable;
struct Gravity;
struct Camera;

// Global ECS coordinator
inline Coordinator coordinator;

// Global Mesh and Shader managers
inline ShaderManager shaderManager;
inline MeshManager meshManager;

class Engine {
public:
    // Initializes engine and components
    void Init() {
        // Init coordinator
        coordinator.Init();
    }

    // Loads and registers components and systems
    void Load() {
        // Reigster components
        coordinator.RegisterComponent<Camera>();
        coordinator.RegisterComponent<Gravity>();
        coordinator.RegisterComponent<Renderable>();
        coordinator.RegisterComponent<RigidBody>();
        coordinator.RegisterComponent<Transform>();

        // Register systems
        // For sure required order: Render -> Transform -> Everything else
        RegisterSystem<RenderSystem, Renderable, Transform>();
        RegisterSystem<TransformSystem, Transform>();
        RegisterSystem<PhysicsSystem, Transform, RigidBody, Gravity>();
        RegisterSystem<CameraSystem, Transform, Camera>();

        // Init all the systems
        // TODO: find a better way to do this?
        auto physicsSystem = std::static_pointer_cast<PhysicsSystem>(systemMap[typeid(PhysicsSystem)]);
        if(physicsSystem) physicsSystem->Init();
        Log::Output(Log::Severity::HAPPY, "PhysicsSystem initialized!");

        auto cameraSystem = std::static_pointer_cast<CameraSystem>(systemMap[typeid(CameraSystem)]);
        if(cameraSystem) cameraSystem->Init();
        Log::Output(Log::Severity::HAPPY, "CameraSystem initialized!");

        auto transformSystem = std::static_pointer_cast<TransformSystem>(systemMap[typeid(TransformSystem)]);
        if(transformSystem) transformSystem->Init();
        Log::Output(Log::Severity::HAPPY, "TransformSystem initialized!");

        auto renderSystem = std::static_pointer_cast<RenderSystem>(systemMap[typeid(RenderSystem)]);
        if(renderSystem) renderSystem->Init();
        Log::Output(Log::Severity::HAPPY, "RenderSystem initialized!");
    }

    // Main loop
    void Update(float dt) {
        // Update all the systems
        // TODO: find a better way to do this?
        auto physicsSystem = std::static_pointer_cast<PhysicsSystem>(systemMap[typeid(PhysicsSystem)]);
        if(physicsSystem) physicsSystem->Update(dt);

        auto cameraSystem = std::static_pointer_cast<CameraSystem>(systemMap[typeid(CameraSystem)]);
        if(cameraSystem) cameraSystem->Update(dt);

        auto transformSystem = std::static_pointer_cast<TransformSystem>(systemMap[typeid(TransformSystem)]);
        if(transformSystem) transformSystem->Update(dt);

        auto renderSystem = std::static_pointer_cast<RenderSystem>(systemMap[typeid(RenderSystem)]);
        if(renderSystem) renderSystem->Update(dt);
    }

    // Registers new systems
    template <typename SystemType, typename... ComponentTypes>
    void RegisterSystem() {
        // Register the system
        auto system = coordinator.RegisterSystem<SystemType>();

        // Create signature
        Signature signature;
        (signature.set(coordinator.GetComponentType<ComponentTypes>()), ...);

        // Set the signature
        coordinator.SetSystemSignature<SystemType>(signature);

        // Store the system
        systemMap[typeid(SystemType)] = system;

        Log::Output(Log::Severity::HAPPY, "A System has been registered!");
    }

private:
    // List of registered systems
    std::unordered_map<std::type_index, std::shared_ptr<System>> systemMap;
};

#endif //ENGINE_HPP
