#ifndef CTK_MATRIX_H
#define CTK_MATRIX_H

#include "../types/types.h"

typedef struct {
    f32 m0, m1, m2, m3;
    f32 m4, m5, m6, m7;
    f32 m8, m9, m10, m11;
    f32 m12, m13, m14, m15;
} Mat4;

#define MAT4_ZERO        \
    ((Mat4) {0, 0, 0, 0, \
             0, 0, 0, 0, \
             0, 0, 0, 0, \
             0, 0, 0, 0})
#define MAT4_IDENTITY    \
    ((Mat4) {1, 0, 0, 0, \
             0, 1, 0, 0, \
             0, 0, 1, 0, \
             0, 0, 0, 1})
#define MAT4_ONE         \
    ((Mat4) {1, 1, 1, 1, \
             1, 1, 1, 1, \
             1, 1, 1, 1, \
             1, 1, 1, 1})

// MODIFY
Mat4 mat4_mul_scalar(const Mat4* self, f32 multiplier);
Mat4 mat4_mul_mat4(const Mat4* self, const Mat4* other);

// QUERY
bool mat4_equals(const Mat4* self, const Mat4* other);

#endif
