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
#include "Core/Engine.hpp"
#include <chrono>
#include <random>

#include "Core/WindowManager.hpp"
#include "ECS/Systems/CameraSystem.hpp"


/*
 * TODO:
 * Transform system and matrix caching
 * Scene system
 * Mesh class
 * Debug system - with imgui
 */

extern Coordinator coordinator;
extern ShaderManager shaderManager;

WindowManager windowManager;
Engine engine;

int main() {
	engine.Init();
    windowManager.Init("BlockGame", 1024, 760, 0, 0);

	engine.Load();

	// Load resources
	shaderManager.AddShader("basic", std::make_unique<Shader>("shaders/basic.vert", "shaders/basic.frag"));

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
			.shaderID = shaderManager.GetShaderID("basic"),
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
    	auto& col = coordinator.GetComponent<Renderable>(cubeEntity);
    	trans.rotation.x += 2 * dt;
    	trans.rotation.y += 3 * dt;
    	trans.rotation.z *= -1.5 * dt;

    	float speed = 0.01f;               // Adjust speed of the color change

    	if (col.color.r > 0.0f && col.color.b <= 0.0f) {
    		col.color.r -= speed; // Red to Yellow to Green
    		col.color.g += speed;
    	} else if (col.color.g > 0.0f && col.color.r <= 0.0f) {
    		col.color.g -= speed; // Green to Cyan to Blue
    		col.color.b += speed;
    	} else if (col.color.b > 0.0f && col.color.g <= 0.0f) {
    		col.color.b -= speed; // Blue to Magenta to Red
    		col.color.r += speed;
    	}

    	engine.Update(dt);

    	windowManager.Update();

        auto stop = std::chrono::high_resolution_clock::now();

        // calculate delta time
        dt = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
    }

    windowManager.Shutdown();

    return EXIT_SUCCESS;
}
