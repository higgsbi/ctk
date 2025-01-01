#include "matrix.h"

Mat4 mat4_mul_scalar(const Mat4* self, f32 multiplier) {
    return (Mat4) {self->m0 * multiplier, self->m1 * multiplier, self->m2 * multiplier, self->m3 * multiplier,
                   self->m4 * multiplier, self->m5 * multiplier, self->m6 * multiplier, self->m7 * multiplier,
                   self->m8 * multiplier, self->m9 * multiplier, self->m10 * multiplier, self->m11 * multiplier,
                   self->m12 * multiplier, self->m13 * multiplier, self->m14 * multiplier, self->m15 * multiplier};
}

/*
REF:

m0, m1, m2, m3;
m4, m5, m6, m7;
m8, m9, m10, m11;
m12, m13, m14, m15;
*/

Mat4 mat4_mul_mat4(const Mat4* self, const Mat4* other) {
    return (Mat4) {
        // row 0
        (self->m0 * other->m0) + (self->m1 * other->m4) + (self->m2 * other->m8) + (self->m3 * other->m12),
        (self->m0 * other->m1) + (self->m1 * other->m5) + (self->m2 * other->m9) + (self->m3 * other->m13),
        (self->m0 * other->m2) + (self->m1 * other->m6) + (self->m2 * other->m10) + (self->m3 * other->m14),
        (self->m0 * other->m3) + (self->m1 * other->m7) + (self->m2 * other->m11) + (self->m3 * other->m15),
        // row 1
        (self->m4 * other->m0) + (self->m5 * other->m4) + (self->m6 * other->m8) + (self->m7 * other->m12),
        (self->m4 * other->m1) + (self->m5 * other->m5) + (self->m6 * other->m9) + (self->m7 * other->m13),
        (self->m4 * other->m2) + (self->m5 * other->m6) + (self->m6 * other->m10) + (self->m7 * other->m14),
        (self->m4 * other->m3) + (self->m5 * other->m7) + (self->m6 * other->m11) + (self->m7 * other->m15),
        // row 2
        (self->m8 * other->m0) + (self->m9 * other->m4) + (self->m10 * other->m8) + (self->m11 * other->m12),
        (self->m8 * other->m1) + (self->m9 * other->m5) + (self->m10 * other->m9) + (self->m11 * other->m13),
        (self->m8 * other->m2) + (self->m9 * other->m6) + (self->m10 * other->m10) + (self->m11 * other->m14),
        (self->m8 * other->m3) + (self->m9 * other->m7) + (self->m10 * other->m11) + (self->m11 * other->m15),
        // row 3
        (self->m12 * other->m0) + (self->m13 * other->m4) + (self->m14 * other->m8) + (self->m15 * other->m12),
        (self->m12 * other->m1) + (self->m13 * other->m5) + (self->m14 * other->m9) + (self->m15 * other->m13),
        (self->m12 * other->m2) + (self->m13 * other->m6) + (self->m14 * other->m10) + (self->m15 * other->m14),
        (self->m12 * other->m3) + (self->m13 * other->m7) + (self->m14 * other->m11) + (self->m15 * other->m15)};
}

bool mat4_equals(const Mat4* self, const Mat4* other) {
    return self->m0 == other->m0 && self->m1 == other->m1 && self->m2 == other->m2 && self->m3 == other->m3 &&
           self->m4 == other->m4 && self->m5 == other->m5 && self->m6 == other->m6 && self->m7 == other->m7 &&
           self->m8 == other->m8 && self->m9 == other->m9 && self->m10 == other->m10 && self->m11 == other->m11 &&
           self->m12 == other->m12 && self->m13 == other->m13 && self->m14 == other->m14 && self->m15 == other->m15;
}
