//
// Created by colli on 1/13/2025.
//

#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <cstdint>
#include <vector>

enum BlockType {
    AIR,
    DIRT,
    GRASS,
    SAND,
    STONE
};

struct Block {
    BlockType type;
    uint8_t light;
};

struct Chunk {
    // 3D Array representation of chunk data
    std::vector<std::vector<std::vector<Block>>> blockData;
};

#endif //CHUNK_HPP
