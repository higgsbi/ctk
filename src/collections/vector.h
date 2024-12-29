#ifndef CTK_VECTOR_H
#define CTK_VECTOR_H

#include "../types/types.h"

#define TYPE_VECTOR(type, type_name, func_name, clone, destroy, compare)                                        \
    typedef struct {                                                                                            \
        type** elements;                                                                                        \
        usize count;                                                                                            \
        usize capacity;                                                                                         \
    } Vec##type_name;                                                                                           \
                                                                                                                \
    typedef type* (*_clone_##func_name)(type*);                                                                 \
    typedef void(_destroy_##func_name)(type*);                                                                  \
    typedef i32(_compare_##func_name)(const type**, const type**);                                              \
    TYPE_OPTION(type*, type_name, func_name, null)                                                              \
                                                                                                                \
    static inline Vec##type_name* vec_##func_name##_new(usize initial_capacity) {                               \
        Vec##type_name* self = (Vec##type_name*) allocate(sizeof(Vec##type_name), initial_capacity);            \
        self->elements = (type**) allocate(sizeof(type*), initial_capacity);                                    \
        self->count = 0;                                                                                        \
        self->capacity = initial_capacity;                                                                      \
        return self;                                                                                            \
    }                                                                                                           \
    static inline void vec_##func_name##_resize(Vec##type_name* self, usize new_capacity) {                     \
        ASSERT_NONNULL(self);                                                                                   \
        self->elements = (type**) reallocate(self->elements, sizeof(type*) * new_capacity);                     \
        self->capacity = new_capacity;                                                                          \
    }                                                                                                           \
    static inline Vec##type_name* vec_##func_name##_clone(Vec##type_name* self) {                               \
        ASSERT_NONNULL(self);                                                                                   \
        Vec##type_name* cloned = vec_##func_name##_new(self->capacity);                                         \
        cloned->count = self->count;                                                                            \
        for (usize i = 0; i < self->count; i++) {                                                               \
            cloned->elements[i] = clone(self->elements[i]);                                                     \
        }                                                                                                       \
        return cloned;                                                                                          \
    }                                                                                                           \
    static inline void vec_##func_name##_free(Vec##type_name* self) {                                           \
        ASSERT_NONNULL(self);                                                                                   \
        for (usize i = 0; i < self->count; i++) {                                                               \
            destroy(self->elements[i]);                                                                         \
        }                                                                                                       \
        free(self->elements);                                                                                   \
        free(self);                                                                                             \
    }                                                                                                           \
    static inline void vec_##func_name##_push_back(Vec##type_name* self, type* element) {                       \
        ASSERT_NONNULL(self);                                                                                   \
        ASSERT_NONNULL(element);                                                                                \
        if (self->count + 1 > self->capacity) {                                                                 \
            vec_##func_name##_resize(self, self->capacity * 2);                                                 \
        }                                                                                                       \
        self->elements[self->count] = element;                                                                  \
        self->count++;                                                                                          \
    }                                                                                                           \
    static inline void vec_##func_name##_push_back_literal(Vec##type_name* self, type element) {                \
        ASSERT_NONNULL(self);                                                                                   \
        vec_##func_name##_push_back(self, clone(&element));                                                     \
    }                                                                                                           \
    static inline Option##type_name vec_##func_name##_get(Vec##type_name* self, usize index) {                  \
        ASSERT_NONNULL(self);                                                                                   \
        if (index >= self->count) {                                                                             \
            return option_##func_name##_empty();                                                                \
        }                                                                                                       \
        return option_##func_name(self->elements[index]);                                                       \
    }                                                                                                           \
    static inline Option##type_name vec_##func_name##_last(Vec##type_name* self) {                              \
        ASSERT_NONNULL(self);                                                                                   \
        if (self->count <= 0) {                                                                                 \
            return option_##func_name##_empty();                                                                \
        }                                                                                                       \
        return option_##func_name(self->elements[self->count - 1]);                                             \
    }                                                                                                           \
    static inline Option##type_name vec_##func_name##_first(Vec##type_name* self) {                             \
        ASSERT_NONNULL(self);                                                                                   \
        if (self->count <= 0) {                                                                                 \
            return option_##func_name##_empty();                                                                \
        }                                                                                                       \
        return option_##func_name(self->elements[0]);                                                           \
    }                                                                                                           \
    static inline Option##type_name vec_##func_name##_remove(Vec##type_name* self, usize index) {               \
        ASSERT_NONNULL(self);                                                                                   \
        if (index >= self->count) {                                                                             \
            return option_##func_name##_empty();                                                                \
        }                                                                                                       \
        type* value = self->elements[index];                                                                    \
        type* cloned = clone(value);                                                                            \
        destroy(value);                                                                                         \
        for (usize i = index; i < self->count - 1; i++) {                                                       \
            self->elements[i] = self->elements[i + 1];                                                          \
        }                                                                                                       \
        self->count--;                                                                                          \
        return option_##func_name(cloned);                                                                      \
    }                                                                                                           \
    static inline Option##type_name vec_##func_name##_pop_back(Vec##type_name* self) {                          \
        return vec_##func_name##_remove(self, self->count - 1);                                                 \
    }                                                                                                           \
    static inline Option##type_name vec_##func_name##_pop_first(Vec##type_name* self) {                         \
        return vec_##func_name##_remove(self, 0);                                                               \
    }                                                                                                           \
    static inline void vec_##func_name##_delete(Vec##type_name* self, usize index) {                            \
        Option##type_name removed = vec_##func_name##_remove(self, index);                                      \
        if (option_##func_name##_is_present(removed)) {                                                         \
            destroy(removed.value);                                                                             \
        }                                                                                                       \
    }                                                                                                           \
    static inline void vec_##func_name##_sort(Vec##type_name* self) {                                           \
        ASSERT_NONNULL(self);                                                                                   \
        qsort(self->elements, self->count, sizeof(type*), (Compare) compare);                                   \
    }                                                                                                           \
    static inline void vec_##func_name##_sort_custom(Vec##type_name* self, _compare_##func_name compare_func) { \
        ASSERT_NONNULL(self);                                                                                   \
        qsort(self->elements, self->count, sizeof(type*), (Compare) compare_func);                              \
    }

#define FOREACH_VECTOR(declaration, vector, body)                                            \
    for (usize _i_##__COUNTER__ = 0; _i_##__COUNTER__ < vector->count; _i_##__COUNTER__++) { \
        declaration = *vector->elements[_i_##__COUNTER__];                                   \
        body                                                                                 \
    }

#endif
