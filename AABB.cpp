#include "AABB.h"

// Default constructor (invalid AABB)
AABB::AABB()
    : min(std::numeric_limits<float>::max()), 
      max(-std::numeric_limits<float>::max()) {}

// Constructor with min/max points
AABB::AABB(const glm::vec3& min, const glm::vec3& max)
    : min(min), max(max) {}

// Check if this AABB intersects another AABB
bool AABB::intersects(const AABB& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
}

// Check if a point is inside the AABB
bool AABB::contains(const glm::vec3& point) const {
    return (point.x >= min.x && point.x <= max.x) &&
           (point.y >= min.y && point.y <= max.y) &&
           (point.z >= min.z && point.z <= max.z);
}
glm::vec3 AABB::getCorner(int index) const {
    static const std::array<glm::vec3, 8> offsets = {
        glm::vec3(0, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(1, 1, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(0, 1, 1),
        glm::vec3(1, 1, 1)
    };

    return glm::vec3(
        min.x + offsets[index].x * (max.x - min.x),
        min.y + offsets[index].y * (max.y - min.y),
        min.z + offsets[index].z * (max.z - min.z)
    );
}

// Expand AABB to include a new point
void AABB::expand(const glm::vec3& point) {
    min = glm::min(min, point);
    max = glm::max(max, point);
}

// Reset AABB to an invalid state
void AABB::reset() {
    min = glm::vec3(std::numeric_limits<float>::max());
    max = glm::vec3(-std::numeric_limits<float>::max());
}

// Print AABB details
void AABB::print() const {
    std::cout << "AABB Min: (" << min.x << ", " << min.y << ", " << min.z << ")";
    std::cout << ", Max: (" << max.x << ", " << max.y << ", " << max.z << ")\n";
}

// Ray-AABB intersection using Slab Method
bool AABB::rayIntersects(const glm::vec3& rayOrigin, const glm::vec3& rayDir, float& tMin, float& tMax) const {
    glm::vec3 invDir = 1.0f / rayDir;  // Compute inverse direction

    float t1 = (min.x - rayOrigin.x) * invDir.x;
    float t2 = (max.x - rayOrigin.x) * invDir.x;
    float t3 = (min.y - rayOrigin.y) * invDir.y;
    float t4 = (max.y - rayOrigin.y) * invDir.y;
    float t5 = (min.z - rayOrigin.z) * invDir.z;
    float t6 = (max.z - rayOrigin.z) * invDir.z;

    // Find the minimum and maximum intersection distances
    tMin = std::max({std::min(t1, t2), std::min(t3, t4), std::min(t5, t6)});
    tMax = std::min({std::max(t1, t2), std::max(t3, t4), std::max(t5, t6)});

    return tMax >= std::max(0.0f, tMin);  // Ensure intersection is in front of ray origin
}

