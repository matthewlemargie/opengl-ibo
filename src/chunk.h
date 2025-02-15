#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <chrono>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <GL/gl.h>
#include "constants.h"

struct Chunk {
    // hash map of chunk position (index) to block ID of position
    // std::unordered_map<int, int> chunkInstanceMap;
    Chunk();
    ~Chunk();
};
std::vector<GLuint> populateChunk();

#endif
