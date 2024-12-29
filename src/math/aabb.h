#ifndef CTK_AABB_H
#define CTK_AABB_H

#include "math.h"
#include "vectors.h"

typedef struct {
    Vec3 lower;
    Vec3 higher;
} AABB;

static inline AABB aabb(Vec3* one, Vec3* two) {
    return (AABB) {
        .lower = vec3(MIN(one->x, two->x), MIN(one->y, two->y), MIN(one->z, two->z)),
        .higher = vec3(MAX(one->x, two->x), MAX(one->y, two->y), MAX(one->z, two->z)),
    };
}

bool aabb_contains(AABB* self, Vec3* point);
AABB aabb_translate(AABB* self, Vec3* translation);
AABB aabb_expand(AABB* self, Vec3* expansion);
bool aabb_intersects(AABB* self, AABB* other);

#endif
