#include "aabb.h"

bool aabb_contains(AABB* self, Vec3* point) {
    return (point->x >= self->lower.x && point->x <= self->higher.x) &&
           (point->y >= self->lower.y && point->y <= self->higher.y) &&
           (point->z >= self->lower.z && point->z <= self->higher.z);
}

AABB aabb_translate(AABB* self, Vec3* translation) {
    Vec3 lower = vec3_add_vec3(&self->lower, translation);
    Vec3 higher = vec3_add_vec3(&self->higher, translation);
    return aabb(&lower, &higher);
}

AABB aabb_expand(AABB* self, Vec3* expansion) {
    Vec3 lower = vec3_sub_vec3(&self->lower, expansion);
    Vec3 higher = vec3_add_vec3(&self->higher, expansion);
    return aabb(&lower, &higher);
}

bool aabb_intersects(AABB* self, AABB* other) {
    return (self->lower.x <= other->higher.x && self->higher.x >= other->lower.x) &&
           (self->lower.y <= other->higher.y && self->higher.y >= other->lower.y) &&
           (self->lower.z <= other->higher.z && self->higher.z >= other->lower.z);
}
