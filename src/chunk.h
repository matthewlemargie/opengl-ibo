#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <cmath>
#include <chrono>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <GL/gl.h>
#include <FastNoiseLite.h>
#include "constants.h"

struct Chunk {
    FastNoiseLite noise;
    Chunk();
    ~Chunk();
    std::vector<GLuint> populateChunk(int xPos, int zPos);
};

#endif
