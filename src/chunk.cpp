#include "chunk.h"

Chunk::Chunk() {
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    noise.SetFrequency(0.006f);  // Use a moderate frequency for wavy hills
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    noise.SetSeed(seed);}

Chunk::~Chunk() {}

std::vector<GLuint> Chunk::populateChunk(int chunkX, int chunkZ) {
    float frequency = 0.006f;  // Set a frequency that makes waves over short distances
    // float amplitude = 0.9f;  // Control the height of the hills (lower = flatter terrain)

    std::vector<GLuint> newBlocks(CHUNK_X_DIM * CHUNK_Y_DIM * CHUNK_Z_DIM, AIR);
    auto start = std::chrono::high_resolution_clock::now();

    // For smooth transitions, adjust the coordinates based on chunk position.
    for (int k = 0; k < CHUNK_X_DIM; ++k) {
        for (int i = 0; i < CHUNK_Z_DIM; ++i) {
            // Adjust the noise coordinates for this chunk, based on chunkX and chunkZ
            float worldX = (chunkX * CHUNK_X_DIM) + k;
            float worldZ = (chunkZ * CHUNK_Z_DIM) + i;

            float value = noise.GetNoise(worldX, worldZ);
            float normalizedValue = (value + 1.0f) / 2.0f;
            
            // Scale the height so that it varies between height * 0.8 and height * 1.0
            float height = value * CHUNK_Y_DIM;  // Adjust the height
            height = 0.8f * CHUNK_Y_DIM + (0.2f) * (height);  // Further scale it between 0.8 and 1.0

            int roundHeight = static_cast<int>(std::ceil(height));
            roundHeight = std::max(1, roundHeight);  // Ensure at least one block is placed

            for (int j = 0; j < roundHeight; ++j) {
                int index = (i * CHUNK_Y_DIM + j) * CHUNK_X_DIM + k;  // Compute the index in the 1D vector
                int newBlockID = GRASS;  // You can vary block IDs here depending on the height
                newBlocks[index] = newBlockID;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    // std::cout << "Chunk populated in " << duration.count() << " seconds" << std::endl;

    return newBlocks;
}


