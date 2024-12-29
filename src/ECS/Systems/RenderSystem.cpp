//
// Created by colli on 12/26/2024.
//

#include "ECS/Systems/RenderSystem.hpp"

#include "ECS/Coordinator.hpp"
#include "ECS/Components/Camera.hpp"
#include "ECS/Components/Renderable.hpp"
#include "ECS/Components/Transform.hpp"

extern Coordinator coordinator;

void RenderSystem::Init() {
    // create shader
    shader = std::make_unique<Shader>("shaders/basic.vert", "shaders/basic.frag");

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

    // Define vertex data
    std::vector<glm::vec3> vertices = {
        // +Z face (front)
        {-0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f},
        {+0.5f, +0.5f, +0.5f}, {-0.5f, +0.5f, +0.5f}, {-0.5f, -0.5f, +0.5f},

        // -Z face (back)
        {-0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f},
        {+0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},

        // +X face (right)
        {+0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f},
        {+0.5f, +0.5f, -0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f},

        // -X face (left)
        {-0.5f, -0.5f, +0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, +0.5f, -0.5f},
        {-0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, +0.5f}, {-0.5f, -0.5f, +0.5f},

        // +Y face (top)
        {-0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, -0.5f},
        {+0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, +0.5f},

        // -Y face (bottom)
        {-0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, -0.5f},
        {+0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, +0.5f}
    };

    // Simplified normal data setup for brevity
    std::vector<glm::vec3> normals = {
        // +Z face
        { 0.0f,  0.0f, +1.0f}, { 0.0f,  0.0f, +1.0f}, { 0.0f,  0.0f, +1.0f},
        { 0.0f,  0.0f, +1.0f}, { 0.0f,  0.0f, +1.0f}, { 0.0f,  0.0f, +1.0f},

        // -Z face
        { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f, -1.0f},
        { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f, -1.0f},

        // +X face
        {+1.0f,  0.0f,  0.0f}, {+1.0f,  0.0f,  0.0f}, {+1.0f,  0.0f,  0.0f},
        {+1.0f,  0.0f,  0.0f}, {+1.0f,  0.0f,  0.0f}, {+1.0f,  0.0f,  0.0f},

        // -X face
        {-1.0f,  0.0f,  0.0f}, {-1.0f,  0.0f,  0.0f}, {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f}, {-1.0f,  0.0f,  0.0f}, {-1.0f,  0.0f,  0.0f},

        // +Y face
        { 0.0f, +1.0f,  0.0f}, { 0.0f, +1.0f,  0.0f}, { 0.0f, +1.0f,  0.0f},
        { 0.0f, +1.0f,  0.0f}, { 0.0f, +1.0f,  0.0f}, { 0.0f, +1.0f,  0.0f},

        // -Y face
        { 0.0f, -1.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}, { 0.0f, -1.0f,  0.0f},
        { 0.0f, -1.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}
    };

    // VAO, VBO setup
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertices
    glGenBuffers(1, &VBO_V);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_V);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)nullptr);
    glEnableVertexAttribArray(0);

    // Normals
    glGenBuffers(1, &VBO_N);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)nullptr);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void RenderSystem::Update(float dt) {
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->SetActive();
    glBindVertexArray(VAO);

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

        glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position) *
                          glm::scale(glm::mat4(1.0f), transform.scale) *
                          glm::yawPitchRoll(transform.rotation.y, transform.rotation.x, transform.rotation.z);

        shader->SetUniform("uModel", model);
        shader->SetUniform("uView", view);
        shader->SetUniform("uProjection", cameraCamera.projectionMatrix);
        shader->SetUniform("uColor", renderable.color);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);
}
