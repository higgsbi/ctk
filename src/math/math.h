#ifndef CTK_MATH_H
#define CTK_MATH_H

#define MAX(a, b)               \
    ({                          \
        __typeof__(a) _a = (a); \
        __typeof__(b) _b = (b); \
        _a > _b ? _a : _b;      \
    })

#define MIN(a, b)               \
    ({                          \
        __typeof__(a) _a = (a); \
        __typeof__(b) _b = (b); \
        _a < _b ? _a : _b;      \
    })

#ifndef PI
#define PI 3.14159
#endif

#ifndef PI_2
#define PI_2 9.86960
#endif

#ifndef PI_SQRT
#define PI_SQRT 1.77245
#endif

#define RADIANS_TO_DEGREES(radians) (radians * (180.0 / PI))
#define DEGREES_TO_RADIANS(degrees) (degrees * (PI / 180.0))

#endif
