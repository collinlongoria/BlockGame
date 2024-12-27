//
// Created by Collin on 12/26/2024.
//

#include "Core/Log.hpp"
#include "ECS/Coordinator.hpp"
#include "Core/WindowManager.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Components/Camera.hpp"
#include "ECS/Components/Renderable.hpp"
#include "ECS/Components/RigidBody.hpp"
#include "ECS/Components/Gravity.hpp"
#include "ECS/Systems/PhysicsSystem.hpp"
#include "ECS/Systems/RenderSystem.hpp"
#include <chrono>
#include <random>

#include "Core/WindowManager.hpp"
#include "ECS/Systems/CameraSystem.hpp"


/*
 * TODO:
 * Make all asserts or cerr into Log
 * Add min/max severity to log
 * finish ECS
 * Render System
 * Camera System
 * Add imgui
 */

Coordinator coordinator;

void HandleInput(const InputEvent& event) {
	for (size_t i = 0; i < event.inputs.size(); ++i) {
		if (event.inputs.test(i)) { // Check if the input is active
			const std::string inputName = GetInputTypeName(static_cast<InputType>(i));
			Log::Output(Log::Severity::MESSAGE, inputName + " is active.");
		}
	}
}

int main() {
    // Init coordinator
    coordinator.Init();

    WindowManager windowManager;
    windowManager.Init("BlockGame", 1024, 760, 0, 0);

	coordinator.AddEventListener<InputEvent>(&HandleInput);

	coordinator.RegisterComponent<Camera>();
	coordinator.RegisterComponent<Gravity>();
	coordinator.RegisterComponent<Renderable>();
	coordinator.RegisterComponent<RigidBody>();
	coordinator.RegisterComponent<Transform>();


	auto physicsSystem = coordinator.RegisterSystem<PhysicsSystem>();
	{
		Signature signature;
		signature.set(coordinator.GetComponentType<Gravity>());
		signature.set(coordinator.GetComponentType<RigidBody>());
		signature.set(coordinator.GetComponentType<Transform>());
		coordinator.SetSystemSignature<PhysicsSystem>(signature);
	}

	physicsSystem->Init();

	auto cameraSystem = coordinator.RegisterSystem<CameraSystem>();
	{
    	Signature signature;
    	signature.set(coordinator.GetComponentType<Camera>());
    	signature.set(coordinator.GetComponentType<Transform>());
    	coordinator.SetSystemSignature<CameraSystem>(signature);
	}

	cameraSystem->Init();

	auto renderSystem = coordinator.RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(coordinator.GetComponentType<Renderable>());
		signature.set(coordinator.GetComponentType<Transform>());
		coordinator.SetSystemSignature<RenderSystem>(signature);
	}

	renderSystem->Init();


	// Create a single red cube entity
	auto cubeEntity = coordinator.CreateEntity();

	coordinator.AddComponent(
		cubeEntity,
		Transform{
			.position = glm::vec3(0.0f, 0.0f, 0.0f), // Centered at the origin
			.rotation = glm::vec3(0.0f, 0.0f, 0.0f), // No rotation
			.scale = glm::vec3(1.0f, 1.0f, 1.0f)     // Unit scale
		});

	coordinator.AddComponent(
		cubeEntity,
		Renderable{
			.color = glm::vec3(1.0f, 0.0f, 0.0f) // Red color
		});

	coordinator.AddComponent(
		cubeEntity,
		Gravity{Vec3(0.0f, 0.f, 0.0f)});

	coordinator.AddComponent(
		cubeEntity,
		RigidBody{});

    float dt = 0.0f;

    // Main loop
    while(!windowManager.Quit()) {
        auto start = std::chrono::high_resolution_clock::now();

    	windowManager.ProcessInput();

    	auto& trans = coordinator.GetComponent<Transform>(cubeEntity);
    	trans.rotation.x += 2 * dt;
    	trans.rotation.y += 3 * dt;

    	physicsSystem->Update(dt);

    	cameraSystem->Update(dt);

    	renderSystem->Update(dt);

    	windowManager.Update();

        auto stop = std::chrono::high_resolution_clock::now();

        // calculate delta time
        dt = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
    }

    windowManager.Shutdown();

    return EXIT_SUCCESS;
}
