#ifndef CTK_VECTORS_H
#define CTK_VECTORS_H

#include "../types/types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4;

typedef Vec4 Quaternion;

#define vec3(x, y, z) ((Vec3) {x, y, z})
#define vec4(x, y, z, w) ((Vec4) {x, y, z, w})
#define VEC3_ZERO ((Vec3) {0.0, 0.0, 0.0})
#define VEC3_ONE ((Vec3) {1.0, 1.0, 1.0})
#define VEC3_UP ((Vec3) {0.0, 1.0, 0.0})
#define VEC4_ZERO ((Vec4) {0.0, 0.0, 0.0, 0.0})
#define VEC4_ONE ((Vec4) {1.0, 1.0, 1.0, 1.0})
#define VEC4_IDENTITY ((Vec4) {0.0, 0.0, 0.0, 1.0})

// VEC3

// MODIFY
Vec3 vec3_add(const Vec3* self, f32 x, f32 y, f32 z);
Vec3 vec3_sub(const Vec3* self, f32 x, f32 y, f32 z);
Vec3 vec3_add_vec3(const Vec3* self, const Vec3* other);
Vec3 vec3_sub_vec3(const Vec3* self, const Vec3* other);
Vec3 vec3_mul(const Vec3* self, f32 scale);
f32 vec3_dot(const Vec3* self, const Vec3* other);
Vec3 vec3_cross(const Vec3* self, const Vec3* other);
Vec3 vec3_normalize(const Vec3* self);
Vec3 vec3_negated(const Vec3* self);

// QUERY
bool vec3_equals(const Vec3* self, const Vec3* other);
bool vec3_equals_rounded(const Vec3* self, const Vec3* other);
f32 vec3_length(const Vec3* self);

// VEC4

static inline Vec4 vec4_from_vec3(const Vec3* self, f32 w) {
    return vec4(self->x, self->y, self->z, w);
}

bool vec4_equals(const Vec4* self, const Vec4* other);
bool vec4_equals_rounded(const Vec4* self, const Vec4* other);

#endif
