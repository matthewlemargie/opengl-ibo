#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <chrono>
#include <unordered_map>
#include <tuple>

#include "blocks.h"

struct Chunk {
    BlockID* chunkblocks;
    // hash map of chunk position (index) to block ID of position
    std::unordered_map<int, int> chunkInstanceMap;

    Chunk();
    ~Chunk();
    std::vector<std::vector<GLfloat>> populateChunk();
};

#endif
