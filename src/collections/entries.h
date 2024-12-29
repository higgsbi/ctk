#ifndef CTK_ENTRIES_H
#define CTK_ENTRIES_H

#include "entry.h"
#include "list.h"

#define TYPE_ENTRIES(type, type_name, func_name, destroy, clone)                                                  \
    TYPE_ENTRY(type, type_name, func_name, destroy, clone)                                                        \
    TYPE_LIST(Entry##type_name, Entry##type_name, entry_##func_name, entry_##func_name##_clone,                   \
              entry_##func_name##_free)                                                                           \
                                                                                                                  \
    typedef struct {                                                                                              \
        ListEntry##type_name** elements;                                                                          \
        usize count;                                                                                              \
        usize capacity;                                                                                           \
    } Entries##type_name;                                                                                         \
                                                                                                                  \
    Entries##type_name* entries_##func_name##_new(usize capacity) {                                               \
        Entries##type_name* entries = (Entries##type_name*) allocate(sizeof(Entries##type_name), 1);              \
        entries->count = 0;                                                                                       \
        entries->capacity = capacity <= 0 ? 1 : capacity;                                                         \
        entries->elements = (ListEntry##type_name**) callocate(sizeof(ListEntry##type_name*), entries->capacity); \
        return entries;                                                                                           \
    }                                                                                                             \
    Entries##type_name* entries_##func_name##_clone(Entries##type_name* self) {                                   \
        ASSERT_NONNULL(self);                                                                                     \
        Entries##type_name* cloned = (Entries##type_name*) allocate(sizeof(Entries##type_name), 1);               \
        cloned->count = self->count;                                                                              \
        cloned->capacity = self->capacity;                                                                        \
        cloned->elements = allocate(sizeof(ListEntry##type_name*), self->capacity);                               \
        for (usize i = 0; i < self->capacity; i++) {                                                              \
            ListEntry##type_name* entries = self->elements[i];                                                    \
            if (entries == null) {                                                                                \
                cloned->elements[i] = null;                                                                       \
                continue;                                                                                         \
            }                                                                                                     \
            cloned->elements[i] = list_entry_##func_name##_clone(self->elements[i]);                              \
        }                                                                                                         \
        return cloned;                                                                                            \
    }                                                                                                             \
    void entries_##func_name##_free(Entries##type_name* self, bool deep) {                                        \
        ASSERT_NONNULL(self);                                                                                     \
        for (usize i = 0; i < self->capacity; i++) {                                                              \
            ListEntry##type_name* entries = self->elements[i];                                                    \
                                                                                                                  \
            if (entries == null) {                                                                                \
                continue;                                                                                         \
            }                                                                                                     \
                                                                                                                  \
            NodeEntry##type_name* iterator = entries->head;                                                       \
            while (iterator != null) {                                                                            \
                if (deep) {                                                                                       \
                    entry_##func_name##_free(iterator->value);                                                    \
                } else {                                                                                          \
                    free(iterator->value);                                                                        \
                }                                                                                                 \
                NodeEntry##type_name* temp = iterator;                                                            \
                iterator = iterator->next;                                                                        \
                free(temp);                                                                                       \
            }                                                                                                     \
            free(entries);                                                                                        \
        }                                                                                                         \
        free(self->elements);                                                                                     \
        free(self);                                                                                               \
    }                                                                                                             \
    ListEntry##type_name* entries_##func_name##_get(Entries##type_name* self, usize index) {                      \
        ASSERT_NONNULL(self);                                                                                     \
        ASSERT_IN_RANGE(index, self->capacity);                                                                   \
        return self->elements[index];                                                                             \
    }                                                                                                             \
    void entries_##func_name##_add(Entries##type_name* self, ListEntry##type_name* elements) {                    \
        ASSERT_NONNULL(self);                                                                                     \
        ASSERT_NONNULL(elements);                                                                                 \
                                                                                                                  \
        if (self->count + 1 > self->capacity) {                                                                   \
            if (self->capacity == 0) {                                                                            \
                self->capacity = 2;                                                                               \
            } else {                                                                                              \
                self->capacity = self->capacity * 2;                                                              \
            }                                                                                                     \
            self->elements = reallocate(self->elements, self->capacity);                                          \
        }                                                                                                         \
                                                                                                                  \
        self->elements[self->count] = elements;                                                                   \
        self->count++;                                                                                            \
    }

#endif
