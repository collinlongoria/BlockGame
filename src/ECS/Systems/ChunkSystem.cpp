//
// Created by colli on 1/13/2025.
//

#include "ECS/Systems/ChunkSystem.hpp"

#include "imgui.h"
#include "ECS/Coordinator.hpp"
#include "ECS/Components/Renderable.hpp"
#include "Graphics/MeshManager.hpp"
#include "Graphics/ShaderManager.hpp"

extern Coordinator coordinator;
extern MeshManager meshManager;
extern ShaderManager shaderManager;
extern Entity cameraEntity;

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

void ChunkSystem::InitChunk(const Entity& entity) {
    auto& chunk = coordinator.GetComponent<Chunk>(entity);

    for (uint32_t i = 0; i < NUM_SUBCHUNKS; i++) {
        // Create an "empty" Mesh
        auto emptyMesh = std::make_unique<Mesh>();
        // Add it to MeshManager
        int32_t meshID = meshManager.AddMesh("chunk_submesh_" + std::to_string((unsigned long)entity) + "_" + std::to_string(i),
                                          std::move(emptyMesh));
        // Store the ID in the chunk
        chunk.subchunkMeshes[i] = meshID;

        MarkSubchunkClean(chunk, i);
    }
}

void ChunkSystem::Update(float dt) {
    for(auto& entity : entities) {
        auto& chunk = coordinator.GetComponent<Chunk>(entity);

        if(!chunk.initialized) {
            InitChunk(entity);
            chunk.initialized = true;
        }

        // For each subchunk, if dirty, regenerate its mesh
        for(uint32_t i = 0; i < NUM_SUBCHUNKS; ++i) {
            if(IsSubchunkDirty(chunk, i)) {
                // Rebuild Mesh
                Mesh newMesh = GenerateSubchunk(chunk, i);
                std::unique_ptr<Mesh> newMeshPtr = std::make_unique<Mesh>(std::move(newMesh));

                int32_t meshID = chunk.subchunkMeshes[i];
                if(meshID != -1) meshManager.ReplaceMesh(meshID, std::move(newMeshPtr));

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

    //////////////////////////
    // RENDER CODE
    /////////////////////////

    auto& cameraTransform = coordinator.GetComponent<Transform>(cameraEntity);
    auto& cameraCamera = coordinator.GetComponent<Camera>(cameraEntity);

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

    // Bind the chunk shader
    auto chunkShader = shaderManager.GetShader(shaderManager.GetShaderID("chunk"));
    chunkShader->Bind();

    // For example, set a uniform directional light:
    chunkShader->SetUniform("lightDir", Vec3(0.3f, -1.0f, 0.2f));
    chunkShader->SetUniform("viewPos", view);

    // Set block colors [0..4], just as an example
    // If you have more than 5 types, expand this array or do a loop
    chunkShader->SetUniform("blockColors[0]", Vec3(0.8f, 0.8f, 0.8f));  // Air => won't be drawn anyway
    chunkShader->SetUniform("blockColors[1]", Vec3(0.5f, 0.3f, 0.1f));  // Dirt
    chunkShader->SetUniform("blockColors[2]", Vec3(0.3f, 0.6f, 0.2f));  // Grass
    chunkShader->SetUniform("blockColors[3]", Vec3(0.9f, 0.9f, 0.5f));  // Sand
    chunkShader->SetUniform("blockColors[4]", Vec3(0.4f, 0.4f, 0.4f));  // Stone

    // Let's gather all chunk entities
    for (auto entity : entities) {
        auto& chunk = coordinator.GetComponent<Chunk>(entity);

        // If you want a chunk transform, you could do:
        // if (coordinator.HasComponent<Transform>(entity)) {...}
        // but for now let's assume chunk is at origin => identity model

        glm::mat4 model = glm::mat4(1.0f);

        // Upload model/view/proj
        chunkShader->SetUniform("uView",  view);
        chunkShader->SetUniform("uProjection", cameraCamera.projectionMatrix);
        chunkShader->SetUniform("uModel", model);

        // Render each subchunk mesh
        for (uint32_t i = 0; i < NUM_SUBCHUNKS; i++) {
            uint32_t meshID = chunk.subchunkMeshes[i];
            Mesh* mesh = meshManager.GetMesh(meshID);
            if (!mesh) continue;

            mesh->Bind();
            mesh->Draw();
            mesh->Unbind();
        }
    }

    glUseProgram(0);
    glBindVertexArray(0);
}

void PlaceBlock(Chunk& chunk, int x, int y, int z, BlockType type) {
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

void RemoveBlock(Chunk& chunk, int x, int y, int z) {
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
