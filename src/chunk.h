#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <chrono>

constexpr int CHUNK_X_DIM = 16;
constexpr int CHUNK_Y_DIM = 16;
constexpr int CHUNK_Z_DIM = 16;

enum BlockID {
    AIR,
    GRASS,
};

struct Chunk {
    BlockID* chunkblocks;

    Chunk();
    ~Chunk();
    void populateChunk();
};

#endif
