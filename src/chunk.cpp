#include "chunk.h"

Chunk::Chunk() : chunkInstanceMap() {
    chunkblocks = new BlockID[CHUNK_X_DIM * CHUNK_Y_DIM * CHUNK_Z_DIM];  // Allocate memory for 3D array in 1D
    populateChunk();
}

    // Destructor
Chunk::~Chunk() {
    delete[] chunkblocks;
}

void Chunk::populateChunk() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < CHUNK_X_DIM; ++i) {
        for (int j = 0; j < CHUNK_Y_DIM; ++j) {
            for (int k = 0; k < CHUNK_Z_DIM; ++k) {
                
                // calculate blockID 
                int ID = GRASS;


                // Calculate the linear index for 3D coordinates
                int index = i + (j * CHUNK_X_DIM) + (k * CHUNK_X_DIM * CHUNK_Y_DIM);
                chunkblocks[index] = GRASS;
                chunkInstanceMap.insert({index, GRASS});
                // glBindVertexArray(ID);
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Chunk populated in " << duration.count() << " seconds" << std::endl;
}
