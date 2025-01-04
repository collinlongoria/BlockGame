//
// Created by Collin on 1/4/2025.
//

#ifndef MESH_HPP
#define MESH_HPP
#include <vector>

#include "Core/Math.hpp"
#include "glad/glad.h"

class Mesh {
public:
    // Constructor
    Mesh(const std::vector<Vec3>& vertices,
         const std::vector<Vec3>& normals,
         const std::vector<Vec2>& uvs,
         const std::vector<unsigned int>& indices)
         : indicesCount(static_cast<GLsizei>(indices.size())) {
        InitMesh(vertices, normals, uvs, indices);
    }

    // Deconstructor
    ~Mesh() {
        glDeleteBuffers(1, &VBO_V);
        glDeleteBuffers(1, &VBO_N);
        glDeleteBuffers(1, &VBO_UV);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    // Binds the mesh for use
    void Bind() const {
        glBindVertexArray(VAO);
    }

    // Unbinds the mesh
    void Unbind() const {
        glBindVertexArray(0);
    }

    // Draws the mesh
    void Draw() const {
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
    }

private:
    // OpenGL objects
    GLuint VAO{}, VBO_V{}, VBO_N{}, VBO_UV{}, EBO{};
    GLsizei indicesCount;

    void InitMesh(const std::vector<glm::vec3>& vertices,
              const std::vector<glm::vec3>& normals,
              const std::vector<glm::vec2>& uvs,
              const std::vector<unsigned int>& indices) {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // vertices
        glGenBuffers(1, &VBO_V);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_V);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);

        // normals
        glGenBuffers(1, &VBO_N);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(1);

        // uvs
        glGenBuffers(1, &VBO_UV);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
        glEnableVertexAttribArray(2);

        // indices
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }
};

#endif //MESH_HPP
