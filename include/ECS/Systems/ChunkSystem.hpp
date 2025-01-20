//
// Created by colli on 1/13/2025.
//

#ifndef CHUNKSYSTEM_HPP
#define CHUNKSYSTEM_HPP
#include "ECS/System.hpp"
#include "ECS/Components/Chunk.hpp"
#include "Graphics/Mesh.hpp"

class ChunkSystem : public System {
public:
    void Init();

    void Update(float dt);

    // Initialize a chunk, to fill mesh IDs
    void InitChunk(Entity& entity);

    // Place a Block
    void PlaceBlock(Chunk& chunk, int x, int y, int z, BlockType type);

    // Remove a block
    void RemoveBlock(Chunk& chunk, int x, int y, int z);

private:
    // Generate mesh for one subchunk
    Mesh GenerateSubchunk(const Chunk& chunk, uint32_t subIndex);
};



#endif //CHUNKSYSTEM_HPP
