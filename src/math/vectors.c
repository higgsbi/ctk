#include "vectors.h"
#include <math.h>

// MODIFY
Vec3 vec3_add(const Vec3* self, f32 x, f32 y, f32 z) {
    return vec3(self->x + x, self->y + y, self->z + z);
}

Vec3 vec3_sub(const Vec3* self, f32 x, f32 y, f32 z) {
    return vec3(self->x - x, self->y - y, self->z - z);
}

Vec3 vec3_add_vec3(const Vec3* self, const Vec3* other) {
    return vec3(self->x + other->x, self->y + other->y, self->z + other->z);
}

Vec3 vec3_sub_vec3(const Vec3* self, const Vec3* other) {
    return vec3(self->x - other->x, self->y - other->y, self->z - other->z);
}

Vec3 vec3_mul(const Vec3* self, f32 scale) {
    return vec3(self->x * scale, self->y * scale, self->z * scale);
}

Vec3 vec3_cross(const Vec3* self, const Vec3* other) {
    return vec3((self->y * other->z) - (self->z * other->y), (self->z * other->x) - (self->x * other->z),
                (self->x * other->y) - (self->y * other->x));
}

f32 vec3_length(const Vec3* self) {
    return sqrt((self->x * self->x) + (self->y * self->y) + (self->z * self->z));
}

Vec3 vec_normalize(const Vec3* self) {
    f32 length = vec3_length(self);
    return vec3(self->x / length, self->y / length, self->z / length);
}

// QUERY
bool vec3_equals(const Vec3* self, const Vec3* other) {
    return self->x == other->x && self->y == other->y && self->z == other->z;
}

bool vec3_equals_rounded(const Vec3* self, const Vec3* other) {
    return (i32) self->x == (i32) other->x && (i32) self->y == (i32) other->y && (i32) self->z == (i32) other->z;
}

// VEC4

bool vec4_equals(const Vec4* self, const Vec4* other) {
    return self->x == other->x && self->y == other->y && self->z == other->z && self->w == other->w;
}

bool vec4_equals_rounded(const Vec4* self, const Vec4* other) {
    return (i32) self->x == (i32) other->x && (i32) self->y == (i32) other->y && (i32) self->z == (i32) other->z && (i32) self->w == (i32) other->w;
}
