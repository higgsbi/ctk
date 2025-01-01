#include "ctk/math/matrix.h"
#include <float.h>
#include "ctk/io.h"

const Mat4 two = {2, 2, 2, 2,
                  2, 2, 2, 2,
                  2, 2, 2, 2,
                  2, 2, 2, 2};

const Mat4 incremental = {1, 2, 3, 4,
                          5, 6, 7, 8,
                          9, 10, 11, 12,
                          13, 14, 15, 16};

const Mat4 result_expected = {20, 20, 20, 20,
                              52, 52, 52, 52,
                              84, 84, 84, 84,
                              116, 116, 116, 116};

int main() {
    Mat4 two_calculated = mat4_mul_scalar(&MAT4_ONE, 2);
    print(&str("2M Hardcoded: %m\n2M Calculated: %m\n"), &two, &two_calculated);
    assert(mat4_equals(&two, &two_calculated));
    print(&str("\n----------------------------\n"));

    Mat4 result = mat4_mul_mat4(&incremental, &two_calculated);
    print(&str("Incremental: %m\n\n(+)\n\n2M Calculated: %m\n\n(=)\n\nResult: %m\n"), &incremental, &two_calculated, &result);
    assert(mat4_equals(&result, &result_expected));

    return 0;
}
