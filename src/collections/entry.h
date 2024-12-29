#ifndef CTK_ENTRY_H
#define CTK_ENTRY_H

#include "../types/string.h"

#define TYPE_ENTRY(type, type_name, func_name, destroy, clone)                                \
    typedef struct {                                                                          \
        Str* key;                                                                             \
        type* value;                                                                          \
    } Entry##type_name;                                                                       \
                                                                                              \
    Entry##type_name* entry_##func_name##_new(Str* key, type* value) {                        \
        Entry##type_name* entry = (Entry##type_name*) allocate(sizeof(Entry##type_name), 1);  \
        entry->key = key;                                                                     \
        entry->value = value;                                                                 \
        return entry;                                                                         \
    }                                                                                         \
    void entry_##func_name##_free(Entry##type_name* self) {                                   \
        str_free_deep(self->key);                                                             \
        destroy(self->value);                                                                 \
        free(self);                                                                           \
    }                                                                                         \
    Entry##type_name* entry_##func_name##_clone(Entry##type_name* self) {                     \
        Entry##type_name* cloned = (Entry##type_name*) allocate(sizeof(Entry##type_name), 1); \
        cloned->key = str_clone_deep(self->key);                                              \
        cloned->value = clone(self->value);                                                   \
        return cloned;                                                                        \
    }

#endif
