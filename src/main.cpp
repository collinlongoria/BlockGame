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

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

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
extern MeshManager meshManager;

WindowManager windowManager;
Engine engine;

int main() {
	engine.Init();
    windowManager.Init("BlockGame", 1024, 760, 0, 0);

	engine.Load();

	// Load resources
	shaderManager.AddShader("basic", std::make_unique<Shader>("shaders/basic.vert", "shaders/basic.frag"));
	meshManager.AddMesh("teapot", std::make_unique<Mesh>("assets/teapot.obj"));

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
			.meshID = meshManager.GetMeshID("teapot"),
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

    	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	    {
        	static float f = 0.0f;
        	static int counter = 0;

        	// Start the Dear ImGui frame
        	ImGui_ImplOpenGL3_NewFrame();
        	ImGui_ImplGlfw_NewFrame();
        	ImGui::NewFrame();

        	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        		counter++;
        	ImGui::SameLine();
        	ImGui::Text("counter = %d", counter);

        	ImGui::End();
	    }

    	auto& trans = coordinator.GetComponent<Transform>(cubeEntity);
    	auto& col = coordinator.GetComponent<Renderable>(cubeEntity);
    	//trans.rotation.x += 2 * dt;
    	//trans.rotation.y += 3 * dt;
    	//trans.rotation.z *= -1.5 * dt;

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

    	ImGui::Render();
    	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        auto stop = std::chrono::high_resolution_clock::now();

        // calculate delta time
        dt = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
    }

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

    windowManager.Shutdown();

    return EXIT_SUCCESS;
}
