#ifndef CHUNK_H
#define CHUNK_H

#include "constants.h"

#include <iostream>
#include <cmath>
#include <chrono>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <GL/gl.h>
#include <FastNoiseLite.h>


struct Chunk {
    FastNoiseLite noise;
    std::pair<std::vector<Vertex>, std::vector<GLuint>> meshData;

    std::vector<int> blocks;

    int c_w_xPos;
    int c_w_zPos;

    Chunk();
    ~Chunk();

    std::vector<int> populateChunk(int chunkX, int chunkZ);
    std::pair<std::vector<Vertex>, std::vector<GLuint>> getMeshData();
};

#endif
