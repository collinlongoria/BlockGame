//
// Created by Collin on 1/4/2025.
//

#include "Graphics/ShaderManager.hpp"

#include "Core/Log.hpp"

uint32_t ShaderManager::AddShader(const std::string &name, std::unique_ptr<Shader> shader) {
    uint32_t id = nextID++;
    shaders[id] = std::move(shader);
    nameToID[name] = id;
    return id;
}

Shader *ShaderManager::GetShader(uint32_t id) {
    if(shaders.find(id) != shaders.end()) {
        return shaders[id].get();
    }
    Log::Output(Log::Severity::ERROR, "Shader doesn't exist (based on ID)");
    return nullptr;
}

uint32_t ShaderManager::GetShaderID(const std::string &name) const {
    auto it = nameToID.find(name);
    if(it != nameToID.end()) {
        return it->second;
    }
    Log::Output(Log::Severity::ERROR, "Shader name has no matching ID.");
    // return invalid ID
    return -1;
}



