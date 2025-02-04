#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>
#include <limits>
#include <array>

class AABB {
public:
    glm::vec3 min;  // Minimum corner of the bounding box
    glm::vec3 max;  // Maximum corner of the bounding box

    // Constructors
    AABB();
    AABB(const glm::vec3& min, const glm::vec3& max);

    // Methods
    glm::vec3 getCorner(int index) const;
    bool intersects(const AABB& other) const;  // Check if two AABBs overlap
    bool contains(const glm::vec3& point) const; // Check if a point is inside
    void expand(const glm::vec3& point); // Expand AABB to include a point
    void reset();  // Reset AABB to an invalid state
    void print() const;  // Print AABB details

    // New: Ray-AABB Intersection Test
    bool rayIntersects(const glm::vec3& rayOrigin, const glm::vec3& rayDir, float& tMin, float& tMax) const;
};

#endif // AABB_H

