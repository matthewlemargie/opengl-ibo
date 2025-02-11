#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <chrono>
#include <unordered_map>
#include <tuple>

#include "blocks.h"

constexpr int CHUNK_X_DIM = 16;
constexpr int CHUNK_Y_DIM = 16;
constexpr int CHUNK_Z_DIM = 16;
constexpr int CHUNK_TOTAL_BLOCKS = CHUNK_X_DIM * CHUNK_Y_DIM * CHUNK_Z_DIM;

enum BlockID {
    AIR = -1,
    GRASS,
    BLOCKID_COUNT,
};

struct Chunk {
    BlockID* chunkblocks;

    // hash map of chunk position (index) to block ID of position
    std::unordered_map<int, int> chunkInstanceMap;

    Chunk();
    ~Chunk();
    void populateChunk();
};

#endif
