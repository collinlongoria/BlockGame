//
// Created by colli on 1/13/2025.
//

#include "ECS/Systems/ChunkSystem.hpp"

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