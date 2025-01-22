//
// Created by Collin on 1/4/2025.
//

#include "Graphics/MeshManager.hpp"

#include "Core/Log.hpp"

int32_t MeshManager::AddMesh(const std::string &name, std::unique_ptr<Mesh> mesh) {
    int32_t id = nextID++;
    meshes[id] = std::move(mesh);
    nameToID[name] = id;

    Log::Output(Log::Severity::HAPPY, "Mesh has been created: " + name + " at ID: " + std::to_string(id));
    return id;
}

Mesh *MeshManager::GetMesh(int32_t id) {
    if(meshes.find(id) != meshes.end()) {
        return meshes[id].get();
    }
    Log::Output(Log::Severity::ERROR, "Mesh does not exist: " + std::to_string(id));
    return nullptr;
}

int32_t MeshManager::GetMeshID(const std::string &name) const {
    auto it = nameToID.find(name);
    if(it != nameToID.end()) {
        return it->second;
    }
    Log::Output(Log::Severity::ERROR, "Mesh name has no matching ID");
    return -1;
}

void MeshManager::ReplaceMesh(int32_t meshID, std::unique_ptr<Mesh> mesh) {
    auto it = meshes.find(meshID);
    if(it == meshes.end()) {
        Log::Output(Log::Severity::WARNING, "Mesh does not exist");
        return;
    }
    it->second = std::move(mesh);
    Log::Output(Log::Severity::HAPPY, "Replaced mesh");
}

