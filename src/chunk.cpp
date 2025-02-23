#include "chunk.h"

Chunk::Chunk()
{
    blocks.resize(CHUNK_TOTAL_BLOCKS);

    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    noise.SetFrequency(0.006f);  // Use a moderate frequency for wavy hills
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    noise.SetSeed(seed);

}

Chunk::~Chunk(){}

std::pair<std::vector<Vertex>, std::vector<GLuint>> Chunk::getMeshData() {
    return meshData;
}

std::vector<int> Chunk::populateChunk(int chunkX, int chunkZ) {
    float frequency = 0.006f;  // Set a frequency that makes waves over short distances
    float amplitude = 0.9f;  // Control the height of the hills (lower = flatter terrain)

    auto start = std::chrono::high_resolution_clock::now();

    // For smooth transitions, adjust the coordinates based on chunk position.
    for (int k = 0; k < CHUNK_X_DIM; ++k) {
        for (int i = 0; i < CHUNK_Z_DIM; ++i) {
            float worldX = (chunkX * CHUNK_X_DIM) + k;
            float worldZ = (chunkZ * CHUNK_Z_DIM) + i;

            // Get noise value and scale to appropriate height range
            float value = noise.GetNoise(worldX, worldZ);
            float height = (value * 0.5f + 0.5f) * CHUNK_Y_DIM * amplitude;
            height = 0.5f * CHUNK_Y_DIM + (0.5f) * (height);

            int roundHeight = static_cast<int>(std::ceil(height));
            roundHeight = std::max(1, roundHeight);  // Ensure at least one block is placed

            // Fill the chunk with blocks based on height
            for (int j = 0; j < CHUNK_Y_DIM; ++j) {
                int index = k + (CHUNK_X_DIM * j) + (CHUNK_X_DIM * CHUNK_Y_DIM * i);
                int newBlockID = (j < roundHeight) ? GRASS : AIR;
                blocks[index] = newBlockID;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    // std::cout << "Chunk populated in " << duration.count() << " seconds" << std::endl;
    return blocks;

}


