#ifndef CTK_VECTORS_H
#define CTK_VECTORS_H

#include "../types/types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

#define vec3(x, y, z) ((Vec3) {x, y, z})

// MODIFY
Vec3 vec3_add(Vec3* self, f32 x, f32 y, f32 z);
Vec3 vec3_sub(Vec3* self, f32 x, f32 y, f32 z);
Vec3 vec3_add_vec3(Vec3* self, Vec3* other);
Vec3 vec3_sub_vec3(Vec3* self, Vec3* other);
Vec3 vec3_mul(Vec3* self, f32 scale);
Vec3 vec3_cross(Vec3* self, Vec3* other);

// QUERY
bool vec3_equals(Vec3* self, Vec3* other);
bool vec3_equals_rounded(Vec3* self, Vec3* other);

#endif
