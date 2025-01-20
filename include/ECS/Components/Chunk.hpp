//
// Created by colli on 1/13/2025.
//

#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <bitset>
#include <cstdint>
#include <vector>

enum class BlockType : uint32_t {
    Air = 0,
    Dirt = 1,
    Grass = 2,
    Sand = 3,
    Stone = 4
};

static constexpr uint32_t CHUNK_SIZE_X = 16;
static constexpr uint32_t CHUNK_SIZE_Y = 256;
static constexpr uint32_t CHUNK_SIZE_Z = 16;

static constexpr uint32_t SUBCHUNK_HEIGHT = 16;
static constexpr uint32_t NUM_SUBCHUNKS = CHUNK_SIZE_Y / SUBCHUNK_HEIGHT;

struct Block {
    BlockType type;
    uint32_t lightLevel;

    Block() : type(BlockType::Air), lightLevel(0) {}
};

// Actual component to be attached to entities
struct Chunk {
    // 3D Array representation of chunk data
    std::vector<Block> blocks;


    // Bitfield to represent the dirtiness of each subchunk
    std::bitset<NUM_SUBCHUNKS> flags;

    // Mesh ID for each chunk
    std::array<uint32_t, NUM_SUBCHUNKS> subchunkMeshes;

    Chunk() {
        // Init all blocks to air
        blocks.resize(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z, Block());

        // Set all to dirty
        flags.set();

        // Init all mesh IDs to -1 (Invalid mesh, won't render)
        subchunkMeshes.fill(-1);
    }
};

// Helper functions
inline uint32_t GetSubchunkIndex(uint32_t blockY) {
    return blockY / SUBCHUNK_HEIGHT;
}

inline void MarkSubchunkDirty(Chunk& chunk, int subIndex) {
    if(subIndex < 0 || subIndex >= NUM_SUBCHUNKS) return;
    chunk.flags.set(subIndex, true);
}

inline bool IsSubchunkDirty(const Chunk& chunk, uint32_t subIndex) {
    if(subIndex < 0 || subIndex >= NUM_SUBCHUNKS) return false;
    return chunk.flags.test(subIndex);
}

inline void MarkSubchunkClean(Chunk& chunk, uint32_t subIndex) {
    if(subIndex < 0 || subIndex >= NUM_SUBCHUNKS) return;
    chunk.flags.set(subIndex, false);
}

inline uint32_t To1DIndex(uint32_t x, uint32_t y, uint32_t z) {
    return x + z * CHUNK_SIZE_X + y * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
}

// Get block at position x,y,z
inline Block GetBlock(const Chunk& chunk, int x, int y, int z) {
    int index = To1DIndex(x, y, z);
    return chunk.blocks[index];
}

// Set block at position x,y,z
inline void SetBlock(Chunk& chunk, int x, int y, int z, BlockType type) {
    int index = To1DIndex(x, y, z);
    chunk.blocks[index].type = type;
}

// Determine if neighbor is air or out of bounds
inline bool IsAirOrOutOfBounds(const Chunk& chunk, int nx, int ny, int nz) {
    if(nx < 0 || nx >= CHUNK_SIZE_X ||
        ny < 0 || ny >= CHUNK_SIZE_Y ||
        nz < 0 || nz >= CHUNK_SIZE_Z ) {
        return true;
    }

    return (GetBlock(chunk, nx, ny, nz).type == BlockType::Air);
}

#endif //CHUNK_HPP
