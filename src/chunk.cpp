#include "chunk.h"

Chunk::Chunk() {
}

Chunk::~Chunk() {
}

std::vector<GLuint> populateChunk() {
    std::vector<GLuint> newBlocks;
    auto start = std::chrono::high_resolution_clock::now();
    for (int k = 0; k < CHUNK_X_DIM; ++k) {
        for (int j = 0; j < CHUNK_Y_DIM; ++j) {
            for (int i = 0; i < CHUNK_Z_DIM; ++i) {
                // choose blockID 
                // need to create world generation function to put here
                
                int newBlockID = GRASS;

                newBlocks.push_back(newBlockID);
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    // std::cout << "Chunk populated in " << duration.count() << " seconds" << std::endl;

    return newBlocks;
}
