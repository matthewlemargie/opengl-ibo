#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr int CHUNK_X_DIM = 16;
constexpr int CHUNK_Y_DIM = 16;
constexpr int CHUNK_Z_DIM = 16;
constexpr int CHUNK_TOTAL_BLOCKS = CHUNK_X_DIM * CHUNK_Y_DIM * CHUNK_Z_DIM;

constexpr int WORLD_X_DIM = 4;
constexpr int WORLD_Z_DIM = 4;
constexpr int WORLD_TOTAL_CHUNKS = WORLD_X_DIM * WORLD_Z_DIM;

constexpr int TOTAL_BLOCKS_RENDERED = WORLD_TOTAL_CHUNKS * CHUNK_TOTAL_BLOCKS;

enum BlockID {
    AIR = -1,
    GRASS,
    BLOCKID_COUNT,
};

#endif
