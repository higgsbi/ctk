#ifndef CTK_TYPES_H
#define CTK_TYPES_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "option.h"

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef char c8;
typedef size_t usize;

typedef void (*Destroy)(void*);
typedef void* (*Clone)(void*);
typedef bool (*Equals)(void*, void*);
typedef i32 (*Compare)(const void*, const void*);

#define null NULL
#define CAST(object, type) ((type) object)
#define LAMBDA(return_type, function_body) ({ return_type __fn__ function_body __fn__; })
#define TEXT_TO_STRING(x) #x
#define STRINGIFY(x) TEXT_TO_STRING(x)

static inline void* reallocate(void* self, usize size) {
    return realloc(self, sizeof(void*) * size);
}

static inline void* allocate(usize size, usize count) {
    return malloc(size * count);
}

static inline void* callocate(usize size, usize count) {
    return calloc(count, size);
}

#define ASSERT_NONNULL(value) assert(value != null)
#define ASSERT_IN_RANGE(index, count) assert(index < count)

static inline i32* int_clone(const i32* self) {
    i32* clone = (i32*) allocate(sizeof(i32), 1);
    *clone = *self;
    return clone;
}

static inline void int_free(i32* self) {
    free(self);
}

static inline i32 int_compare(const i32** self, const i32** other) {
    return **self - **other;
}

static inline f32* float_clone(const f32* self) {
    f32* clone = (f32*) allocate(sizeof(f32), 1);
    *clone = *self;
    return clone;
}

static inline void float_free(f32* self) {
    free(self);
}

static inline i32 float_compare(const f32** self, const f32** other) {
    return **self - **other;
}

#endif
