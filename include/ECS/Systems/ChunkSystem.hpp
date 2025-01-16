//
// Created by colli on 1/13/2025.
//

#ifndef CHUNKSYSTEM_HPP
#define CHUNKSYSTEM_HPP
#include "ECS/Components/Chunk.hpp"

class ChunkSystem {
public:
    void Init();

    void Update(float dt);

    // Place a Block
    void PlaceBlock(Chunk& chunk, int x, int y, int z, BlockType type);

    // Remove a block
    void RemoveBlock(Chunk& chunk, int x, int y, int z);

private:
};



#endif //CHUNKSYSTEM_HPP
