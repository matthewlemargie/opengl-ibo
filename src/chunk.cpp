#include "chunk.h"

Chunk::Chunk() : chunkInstanceMap() {
    chunkblocks = new BlockID[CHUNK_X_DIM * CHUNK_Y_DIM * CHUNK_Z_DIM];  // Allocate memory for 3D array in 1D
}

    // Destructor
Chunk::~Chunk() {
    delete[] chunkblocks;
}

std::vector<std::vector<GLfloat>> Chunk::populateChunk() {
    std::vector<std::vector<GLfloat>> newBlockInstances;
    for (int i = 0; i < BLOCKID_COUNT; ++i) {
        std::vector<GLfloat> blockInstanceVec = {};
        newBlockInstances.push_back(blockInstanceVec);
    }
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < CHUNK_X_DIM; ++i) {
        for (int j = 0; j < CHUNK_Y_DIM; ++j) {
            for (int k = 0; k < CHUNK_Z_DIM; ++k) {
                
                // choose blockID 
                int newBlockID = GRASS;


                // Calculate the linear index for 3D coordinates
                int index = i + (j * CHUNK_X_DIM) + (k * CHUNK_X_DIM * CHUNK_Y_DIM);
                // chunkblocks[index] = GRASS;
                // chunkInstanceMap.insert({index, GRASS});
                newBlockInstances[newBlockID].push_back(i);
                newBlockInstances[newBlockID].push_back(j);
                newBlockInstances[newBlockID].push_back(k);
            }
        }
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Chunk populated in " << duration.count() << " seconds" << std::endl;

    return newBlockInstances;
}
