//
// Created by Collin on 1/4/2025.
//

#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP
#include <memory>
#include <string>

#include "Shader.hpp"


class ShaderManager {
public:
    // Add a shader to the shader manager
    uint32_t AddShader(const std::string& name, std::unique_ptr<Shader> shader);

    // Gets a shader program based on it's ID
    Shader* GetShader(uint32_t id);

    // Gets a shader ID based on shader program name
    uint32_t GetShaderID(const std::string& name) const;

private:
    // next available ID for a shader program to register to
    uint32_t nextID = 0;

    // Map of shader programs to unique IDs
    std::unordered_map<uint32_t, std::unique_ptr<Shader>> shaders;

    // Lookup map for the ID of programs based on program name
    std::unordered_map<std::string, uint32_t> nameToID;
};



#endif //SHADERMANAGER_HPP
