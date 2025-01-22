//
// Created by Collin on 1/4/2025.
//

#ifndef MESHMANAGER_HPP
#define MESHMANAGER_HPP
#include <memory>
#include <string>

#include "Mesh.hpp"


class MeshManager {
public:

    int32_t AddMesh(const std::string& name, std::unique_ptr<Mesh> mesh);

    Mesh* GetMesh(int32_t id);

    int32_t GetMeshID(const std::string& name) const;

    // Overwrites an existing mesh with a new mesh
    void ReplaceMesh(int32_t meshID, std::unique_ptr<Mesh> mesh);

private:

    int32_t nextID = 0;

    std::unordered_map<int32_t, std::unique_ptr<Mesh>> meshes;

    std::unordered_map<std::string, int> nameToID;
};



#endif //MESHMANAGER_HPP
