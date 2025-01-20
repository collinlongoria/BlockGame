//
// Created by colli on 1/13/2025.
//

#include "ECS/Systems/ChunkSystem.hpp"

#include "imgui.h"
#include "ECS/Coordinator.hpp"
#include "ECS/Components/Renderable.hpp"
#include "Graphics/MeshManager.hpp"

extern Coordinator coordinator;
extern MeshManager meshManager;

// Useful definitions related to mesh generation
static const Vec3 FACE_DIRS[6] {
    {1,0,0},
    {-1,0,0},
    {0,1,0},
    {0,-1,0},
    {0,0,1},
    {0,0,-1}
};

static const Vec3 FACE_NORMALS[6] {
    {1,0,0},
    {-1,0,0},
    {0,1,0},
    {0,-1,0},
    {0,0,1},
    {0,0,-1}
};

static const Vec3 FACE_VERT_OFFSETS[6][4] {
    // +X
    {{1,0,0},{1,1,0},{1,1,1},{1,0,1}},
    // -X
    {{0,0,1},{0,1,1},{0,1,0},{0,0,0}},
    // +Y
    {{0,1,0},{1,1,0},{1,1,1},{0,1,1}},
    // -Y
    {{0,0,1},{1,0,1},{1,0,0},{0,0,0}},
    // +Z
    {{0,0,1},{0,1,1},{1,1,1},{1,0,1}},
    // -Z
    {{1,0,0},{1,1,0},{0,1,0},{0,0,0}}
};

//////////////////////////////////////////
// ChunkSystem
//////////////////////////////////////////

void ChunkSystem::Init() {
    // No initialization
}

void ChunkSystem::InitChunk(Entity& entity) {
    auto& chunk = coordinator.GetComponent<Chunk>(entity);

    for (uint32_t i = 0; i < NUM_SUBCHUNKS; i++) {
        // Create an "empty" Mesh
        auto emptyMesh = std::make_unique<Mesh>();
        // Add it to MeshManager
        uint32_t meshID = meshManager.AddMesh("chunk_submesh_" + std::to_string((unsigned long)entity) + "_" + std::to_string(i),
                                          std::move(emptyMesh));
        // Store the ID in the chunk
        chunk.subchunkMeshes[i] = meshID;
    }
}

void ChunkSystem::Update(float dt) {
    for(auto& entity : entities) {
        auto& chunk = coordinator.GetComponent<Chunk>(entity);

        // For each subchunk, if dirty, regenerate its mesh
        for(uint32_t i = 0; i < NUM_SUBCHUNKS; ++i) {
            if(IsSubchunkDirty(chunk, i)) {
                // Rebuild Mesh
                Mesh newMesh = GenerateSubchunk(chunk, i);
                std::unique_ptr<Mesh> newMeshPtr = std::make_unique<Mesh>(std::move(newMesh));

                uint32_t meshID = chunk.subchunkMeshes[i];
                meshManager.ReplaceMesh(meshID, std::move(newMeshPtr));

                MarkSubchunkClean(chunk, i);
            }
        }

        // Render meshes
        for (uint32_t i = 0; i < NUM_SUBCHUNKS; i++) {
            uint32_t meshID = chunk.subchunkMeshes[i];
            Mesh* mesh = meshManager.GetMesh(meshID);
            if (mesh) {
                mesh->Bind();
                mesh->Draw();
                mesh->Unbind();
            }
        }
    }
}

void ChunkSystem::PlaceBlock(Chunk& chunk, int x, int y, int z, BlockType type) {
    if(GetBlock(chunk, x, y, z).type == type) return;

    SetBlock(chunk, x, y, z, type);

    // Mark subchunk dirty
    int subIndex = GetSubchunkIndex(y);
    MarkSubchunkDirty(chunk, subIndex);

    // If block is on boundary, mark dirty
    if(y % SUBCHUNK_HEIGHT == 0 && subIndex > 0) {
        MarkSubchunkDirty(chunk, subIndex - 1);
    }
    else if((y + 1) % SUBCHUNK_HEIGHT == 0 && subIndex < NUM_SUBCHUNKS - 1) {
        MarkSubchunkDirty(chunk, subIndex + 1);
    }
}

void ChunkSystem::RemoveBlock(Chunk& chunk, int x, int y, int z) {
    if(GetBlock(chunk, x, y, z).type == BlockType::Air) return;

    SetBlock(chunk, x, y, z, BlockType::Air);

    int subIndex = GetSubchunkIndex(y);
    MarkSubchunkDirty(chunk, subIndex);

    if(y % SUBCHUNK_HEIGHT == 0 && subIndex > 0) {
        MarkSubchunkDirty(chunk, subIndex - 1);
    }
    else if((y + 1) % SUBCHUNK_HEIGHT == 0 && subIndex < NUM_SUBCHUNKS - 1) {
        MarkSubchunkDirty(chunk, subIndex + 1);
    }
}

Mesh ChunkSystem::GenerateSubchunk(const Chunk &chunk, uint32_t subIndex) {
    // Y range for this subchunk
    uint32_t yStart = subIndex * SUBCHUNK_HEIGHT;
    uint32_t yEnd = yStart * SUBCHUNK_HEIGHT;
    if(yEnd > CHUNK_SIZE_Y) {
        yEnd = CHUNK_SIZE_Y;
    }

    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    // For now, because I have no UV, I am storing
    // blockID in uv.x, keep uv.y = 0
    std::vector<Vec2> uvs;
    std::vector<unsigned int> indices;

    // Face culling loop
    for(uint32_t y = yStart; y < yEnd; ++y) {
        for(uint32_t z = 0; z < CHUNK_SIZE_Z; ++z) {
            for(uint32_t x = 0; x < CHUNK_SIZE_X; ++x) {
                Block block = GetBlock(chunk, x, y, z);
                if(block.type == BlockType::Air) {
                    continue;
                }

                // Convert block type to a float ID
                float blockID = static_cast<float>(static_cast<uint32_t>(block.type));

                // Check 6 directions
                for(int face = 0; face < 6; ++face) {
                    int nx = x + (int)FACE_DIRS[face].x;
                    int ny = y + (int)FACE_DIRS[face].y;
                    int nz = z + (int)FACE_DIRS[face].z;

                    // If neighbor is air/out of bounds -> add this face
                    if(IsAirOrOutOfBounds(chunk, nx, ny, nz)) {
                        unsigned int startIndex = (unsigned int)vertices.size();

                        // Add 4 verts
                        for(int v = 0; v < 4; ++v) {
                            Vec3 offset = FACE_VERT_OFFSETS[face][v];
                            Vec3 pos(
                                (float)x + offset.x,
                                (float)y + offset.y,
                                (float)z + offset.z
                                );

                            vertices.push_back(pos);
                            normals.push_back(FACE_NORMALS[face]);

                            // store blockID, for now
                            uvs.push_back(Vec2(blockID, 0.0f));
                        }

                        // 2 triangles -> 6 indices
                        indices.push_back(startIndex + 0);
                        indices.push_back(startIndex + 1);
                        indices.push_back(startIndex + 2);
                        indices.push_back(startIndex + 2);
                        indices.push_back(startIndex + 3);
                        indices.push_back(startIndex + 0);
                    }
                } // end face loop
            }
        }
    }

    // Construct the mesh
    Mesh mesh(vertices, normals, uvs, indices);
    return mesh;
}
