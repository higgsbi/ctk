#include "vectors.h"

#define VEC3_ZERO ((Vec) { 0.0, 0.0, 0.0}
#define VEC3_ONE ((Vec) { 1.0, 1.0, 1.0}

// MODIFY
Vec3 vec3_add(Vec3* self, f32 x, f32 y, f32 z) {
    return vec3(self->x + x, self->y + y, self->z + z);
}

Vec3 vec3_sub(Vec3* self, f32 x, f32 y, f32 z) {
    return vec3(self->x - x, self->y - y, self->z - z);
}

Vec3 vec3_add_vec3(Vec3* self, Vec3* other) {
    return vec3(self->x + other->x, self->y + other->y, self->z + other->z);
}

Vec3 vec3_sub_vec3(Vec3* self, Vec3* other) {
    return vec3(self->x - other->x, self->y - other->y, self->z - other->z);
}

Vec3 vec3_mul(Vec3* self, f32 scale) {
    return vec3(self->x * scale, self->y * scale, self->z * scale);
}

Vec3 vec3_cross(Vec3* self, Vec3* other) {
    return vec3((self->y * other->z) - (self->z * other->y), (self->z * other->x) - (self->x * other->z),
                (self->x * other->y) - (self->y * other->x));
}

// QUERY
bool vec3_equals(Vec3* self, Vec3* other) {
    return self->x == other->x && self->y == other->y && self->z == other->z;
}

bool vec3_equals_rounded(Vec3* self, Vec3* other) {
    return (i32) self->x == (i32) other->x && (i32) self->y == (i32) other->y && (i32) self->z == (i32) other->z;
}
