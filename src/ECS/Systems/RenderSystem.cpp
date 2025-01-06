//
// Created by colli on 12/26/2024.
//

#include "ECS/Systems/RenderSystem.hpp"

#include "ECS/Coordinator.hpp"
#include "ECS/Components/Camera.hpp"
#include "ECS/Components/Renderable.hpp"
#include "ECS/Components/Transform.hpp"
#include "Graphics/MeshManager.hpp"
#include "Graphics/ShaderManager.hpp"

extern Coordinator coordinator;
extern ShaderManager shaderManager;
extern MeshManager meshManager;

void RenderSystem::Init() {

    camera = coordinator.CreateEntity();

    coordinator.AddComponent(
        camera,
        Transform{
            .position = Vec3(0.0f, 0.0f, 0.0f)
        }
    );

    coordinator.AddComponent(
        camera,
        Camera{
            .projectionMatrix = Camera::ProjectionMatrix(45.0f, 0.1f, 1000.0f, 1024, 760)
        }
        );

}

void RenderSystem::Update(float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto& cameraTransform = coordinator.GetComponent<Transform>(camera);
    auto& cameraCamera = coordinator.GetComponent<Camera>(camera);

    // Calculate the forward direction based on the camera's orientation
    Vec3 forward = glm::normalize(glm::vec3(
     sin(glm::radians(cameraTransform.rotation.y)) * cos(glm::radians(cameraTransform.rotation.x)), // Swap sin/cos for OpenGL
    sin(glm::radians(cameraTransform.rotation.x)),
    -cos(glm::radians(cameraTransform.rotation.y)) * cos(glm::radians(cameraTransform.rotation.x))// Negative Z for forward
    ));

    // Calculate the up direction based on the camera's orientation
    Vec3 right = glm::normalize(glm::cross(forward, Vec3(0.0f, 1.0f, 0.0f))); // Right vector
    Vec3 up = glm::normalize(glm::cross(right, forward));                     // Corrected up vector

    // Use the transform position and forward vector for the view matrix
    glm::mat4 view = glm::lookAt(
        cameraTransform.position,                // Camera position
        cameraTransform.position + forward,      // Camera target (position + forward direction)
        up                                       // Camera up direction
    );

    for (auto const& entity : entities)
    {
        auto const& transform = coordinator.GetComponent<Transform>(entity);
        auto const& renderable = coordinator.GetComponent<Renderable>(entity);

        auto shader = shaderManager.GetShader(renderable.shaderID);
        auto mesh = meshManager.GetMesh(renderable.meshID);

        mesh->Bind();

        Mat4 model = transform.matrix;

        shader->Bind();

        // TODO: smarter system for Uniforms
        shader->SetUniform("uModel", model);
        shader->SetUniform("uView", view);
        shader->SetUniform("viewPos", cameraTransform.position);
        shader->SetUniform("uProjection", cameraCamera.projectionMatrix);
        shader->SetUniform("uColor", renderable.color);

        mesh->Draw();
        mesh->Unbind(); // may not be needed?
    }

    glUseProgram(0);
    glBindVertexArray(0);
}
