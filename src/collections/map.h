#ifndef CTK_MAP_H
#define CTK_MAP_H

#include "../types/string.h"
#include "entries.h"
#include "vector.h"

#ifndef MAP_LOAD_SIZE
#define MAP_LOAD_SIZE 0.75
#endif

usize map_index_from_key(const Str* key, usize entry_capacity) {
    usize divisor = entry_capacity == 0 ? 1 : entry_capacity;
    return str_hash(key) % divisor;
}

#define TYPE_MAP(type, type_name, func_name, destroy, clone)                                                           \
    TYPE_ENTRIES(type, type_name, func_name, destroy, clone)                                                           \
    TYPE_OPTION(type*, type_name, func_name, null)                                                                     \
                                                                                                                       \
    typedef struct {                                                                                                   \
        Entries##type_name* entries;                                                                                   \
        usize count;                                                                                                   \
    } Map##type_name;                                                                                                  \
                                                                                                                       \
    i32 __entry_##func_name##_compare(const Entry##type_name** self, const Entry##type_name** other) {                 \
        return str_compare((const Str**) &((*self)->key), (const Str**) &((*other)->key));                             \
    }                                                                                                                  \
    Entry##type_name* __entry_##func_name##_clone(Entry##type_name* self) {                                            \
        return self;                                                                                                   \
    }                                                                                                                  \
    void __entry_##func_name##_free(Entry##type_name* self) {                                                          \
    }                                                                                                                  \
                                                                                                                       \
    TYPE_VECTOR(Entry##type_name, Entry##type_name, entry_##func_name, __entry_##func_name##_clone,                    \
                __entry_##func_name##_free, __entry_##func_name##_compare)                                             \
                                                                                                                       \
    typedef struct {                                                                                                   \
        VecEntry##type_name* entries;                                                                                  \
        usize count;                                                                                                   \
    } MapSlice##type_name;                                                                                             \
                                                                                                                       \
    void __map_##func_name##_insert(Map##type_name* self, Entries##type_name* entries, Str* key, type* value,          \
                                    bool rehash);                                                                      \
    void __map_##func_name##_rehash(Map##type_name* self);                                                             \
                                                                                                                       \
    Map##type_name* map_##func_name##_new(usize initial_capacity) {                                                    \
        Map##type_name* self = allocate(sizeof(Entries##type_name), 1);                                                \
        self->entries = entries_##func_name##_new(initial_capacity);                                                   \
        self->count = 0;                                                                                               \
        return self;                                                                                                   \
    }                                                                                                                  \
    void map_##func_name##_free(Map##type_name* self) {                                                                \
        entries_##func_name##_free(self->entries, true);                                                               \
        free(self);                                                                                                    \
    }                                                                                                                  \
    Map##type_name* map_##func_name##_clone(Map##type_name* self) {                                                    \
        Map##type_name* cloned = allocate(sizeof(Entries##type_name), 1);                                              \
        cloned->entries = entries_##func_name##_clone(self->entries);                                                  \
        cloned->count = self->count;                                                                                   \
        return cloned;                                                                                                 \
    }                                                                                                                  \
    OptionEntry##type_name map_##func_name##_get(Map##type_name* self, Str* key) {                                     \
        ASSERT_NONNULL(self);                                                                                          \
        ASSERT_NONNULL(key);                                                                                           \
                                                                                                                       \
        usize index = map_index_from_key(key, self->entries->capacity);                                                \
        if (index > self->entries->capacity) {                                                                         \
            return option_entry_##func_name##_empty();                                                                 \
        }                                                                                                              \
                                                                                                                       \
        ListEntry##type_name* list = entries_##func_name##_get(self->entries, index);                                  \
        for (NodeEntry##type_name* i = list->head; i != null; i = i->next) {                                           \
            if (str_equals_str(i->value->key, key)) {                                                                  \
                return option_entry_##func_name(i->value);                                                             \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        return option_entry_##func_name##_empty();                                                                     \
    }                                                                                                                  \
    Option##type_name map_##func_name##_get_value(Map##type_name* self, Str* key) {                                    \
        OptionEntry##type_name entry = map_##func_name##_get(self, key);                                               \
        return entry.present ? option_##func_name(entry.value->value) : option_##func_name##_empty();                  \
    }                                                                                                                  \
    void map_##func_name##_insert_literal(Map##type_name* self, Str* key_owned, type value_literal) {                  \
        __map_##func_name##_insert(self, self->entries, key_owned, clone(&value_literal), true);                       \
    }                                                                                                                  \
    void map_##func_name##_insert_owned(Map##type_name* self, Str* key_owned, type* value_owned) {                     \
        __map_##func_name##_insert(self, self->entries, key_owned, value_owned, true);                                 \
    }                                                                                                                  \
    OptionEntry##type_name map_##func_name##_remove(Map##type_name* self, Str* key) {                                  \
        ASSERT_NONNULL(self);                                                                                          \
        ASSERT_NONNULL(key);                                                                                           \
                                                                                                                       \
        usize index = map_index_from_key(key, self->entries->capacity);                                                \
        if (index > self->entries->capacity) {                                                                         \
            return option_entry_##func_name##_empty();                                                                 \
        }                                                                                                              \
                                                                                                                       \
        ListEntry##type_name* list = entries_##func_name##_get(self->entries, index);                                  \
        if (list == null) {                                                                                            \
            return option_entry_##func_name##_empty();                                                                 \
        }                                                                                                              \
        usize entry_index = 0;                                                                                         \
        for (NodeEntry##type_name* i = list->head; i != null; i = i->next, entry_index++) {                            \
            if (str_equals_str(i->value->key, key)) {                                                                  \
                Entry##type_name* entry = i->value;                                                                    \
                                                                                                                       \
                if (entry_index > 0) {                                                                                 \
                    NodeEntry##type_name* removed = list_entry_##func_name##_pop_index(list, entry_index).value;       \
                    Entry##type_name* entry = removed->value;                                                          \
                    free(removed);                                                                                     \
                    self->count--;                                                                                     \
                    return option_entry_##func_name(entry);                                                            \
                } else if (list->head->next != null) {                                                                 \
                    NodeEntry##type_name* removed = list_entry_##func_name##_pop_head(list).value;                     \
                    Entry##type_name* entry = removed->value;                                                          \
                    free(removed);                                                                                     \
                    self->count--;                                                                                     \
                    return option_entry_##func_name(entry);                                                            \
                } else {                                                                                               \
                    NodeEntry##type_name* head = list_entry_##func_name##_pop_head(list).value;                        \
                    Entry##type_name* entry = head->value;                                                             \
                    free(head);                                                                                        \
                    free(list);                                                                                        \
                    self->entries->elements[index] = null;                                                             \
                    self->count--;                                                                                     \
                    return option_entry_##func_name(entry);                                                            \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
        return option_entry_##func_name##_empty();                                                                     \
    }                                                                                                                  \
    bool map_##func_name##_delete(Map##type_name* self, Str* key) {                                                    \
        OptionEntry##type_name removed = map_##func_name##_remove(self, key);                                          \
        if (removed.present) {                                                                                         \
            entry_##func_name##_free(removed.value);                                                                   \
            return true;                                                                                               \
        }                                                                                                              \
        return false;                                                                                                  \
    }                                                                                                                  \
    MapSlice##type_name map_slice_##func_name##_new(Map##type_name* self) {                                            \
        VecEntry##type_name* entries = vec_entry_##func_name##_new(self->count);                                       \
                                                                                                                       \
        for (usize i = 0; i < self->entries->capacity; i++) {                                                          \
            ListEntry##type_name* list = self->entries->elements[i];                                                   \
            if (list == null) {                                                                                        \
                continue;                                                                                              \
            }                                                                                                          \
                                                                                                                       \
            for (NodeEntry##type_name* i = list_entry_##func_name##_begin(list); list_entry_##func_name##_has_next(i); \
                 i = list_entry_##func_name##_next(i)) {                                                               \
                vec_entry_##func_name##_push_back(entries, i->value);                                                  \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        return (MapSlice##type_name) {.entries = entries, .count = self->count};                                       \
    }                                                                                                                  \
    MapSlice##type_name map_slice_##func_name##_free(MapSlice##type_name* self) {                                      \
        free(self->entries->elements);                                                                                 \
        free(self->entries);                                                                                           \
    }                                                                                                                  \
    void __map_##func_name##_insert(Map##type_name* self, Entries##type_name* entries, Str* key, type* value,          \
                                    bool rehash) {                                                                     \
        usize index = map_index_from_key(key, self->entries->capacity);                                                \
                                                                                                                       \
        /* Existing hash found*/                                                                                       \
        if (entries->elements[index] != null) {                                                                        \
            ListEntry##type_name* list = entries->elements[index];                                                     \
            Entry##type_name* existing = list->head->value;                                                            \
                                                                                                                       \
            /* Duplicates found */                                                                                     \
            if (str_equals_str(existing->key, key)) {                                                                  \
                str_free_deep(existing->key);                                                                          \
                destroy(existing->value);                                                                              \
                existing->key = key;                                                                                   \
                existing->value = value;                                                                               \
            } else {                                                                                                   \
                list_entry_##func_name##_push_back(list, entry_##func_name##_new(key, value));                         \
                self->count++;                                                                                         \
            }                                                                                                          \
                                                                                                                       \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        /* Nothing existing for hash, create new entry */                                                              \
        ListEntry##type_name* list = list_entry_##func_name##_new();                                                   \
        list_entry_##func_name##_push_back(list, entry_##func_name##_new(key, value));                                 \
        bool changed_capacity = false;                                                                                 \
                                                                                                                       \
        if (rehash && (((float) self->count / MAP_LOAD_SIZE) + 1) > entries->capacity) {                               \
            changed_capacity = true;                                                                                   \
            usize old_capacity = entries->capacity;                                                                    \
                                                                                                                       \
            if (entries->capacity == 0) {                                                                              \
                entries->capacity = 2;                                                                                 \
            } else {                                                                                                   \
                entries->capacity = entries->capacity * 2;                                                             \
            }                                                                                                          \
                                                                                                                       \
            entries->elements = reallocate(entries->elements, sizeof(void*) * entries->capacity);                      \
                                                                                                                       \
            /* Clear new memory */                                                                                     \
            for (usize i = old_capacity; i < entries->capacity; i++) {                                                 \
                entries->elements[i] = null;                                                                           \
            }                                                                                                          \
        }                                                                                                              \
        entries->elements[index] = list;                                                                               \
        entries->count++;                                                                                              \
        self->count++;                                                                                                 \
                                                                                                                       \
        if (rehash && changed_capacity) {                                                                              \
            __map_##func_name##_rehash(self);                                                                          \
        }                                                                                                              \
    }                                                                                                                  \
    void __map_##func_name##_rehash(Map##type_name* self) {                                                            \
        usize original_count = self->count;                                                                            \
        Entries##type_name* entries_rehashed = entries_##func_name##_new(self->entries->capacity);                     \
                                                                                                                       \
        for (usize i = 0; i < entries_rehashed->capacity; i++) {                                                       \
            entries_rehashed->elements[i] = null;                                                                      \
        }                                                                                                              \
                                                                                                                       \
        /* Place old entries into new entries with rehash */                                                           \
        for (usize i = 0; i < self->entries->capacity; i++) {                                                          \
            ListEntry##type_name* list = self->entries->elements[i];                                                   \
            if (list == null) {                                                                                        \
                continue;                                                                                              \
            }                                                                                                          \
                                                                                                                       \
            for (NodeEntry##type_name* i = list_entry_##func_name##_begin(list); list_entry_##func_name##_has_next(i); \
                 i = list_entry_##func_name##_next(i)) {                                                               \
                Entry##type_name* entry = i->value;                                                                    \
                __map_##func_name##_insert(self, entries_rehashed, entry->key, entry->value, false);                   \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        /* replace old entries with rehashed entries*/                                                                 \
        entries_##func_name##_free(self->entries, false);                                                              \
        self->entries = entries_rehashed;                                                                              \
        self->count = original_count;                                                                                  \
    }

#define FOREACH_MAP(declaration, map, body)                                                                            \
    for (usize _i_##__COUNTER__ = 0; _i_##__COUNTER__ < map->entries->capacity; _i_##__COUNTER__++) {                  \
        typeof((map)->entries->elements[0]) _list_##__COUNTER__ = map->entries->elements[_i_##__COUNTER__];            \
        if (_list_##__COUNTER__ == null) {                                                                             \
            continue;                                                                                                  \
        }                                                                                                              \
                                                                                                                       \
        for (typeof(_list_##__COUNTER__->head) _j_##__COUNTER__ = _list_##__COUNTER__->head; _j_##__COUNTER__ != null; \
             _j_##__COUNTER__ = _j_##__COUNTER__->next) {                                                              \
            declaration = *_j_##__COUNTER__->value;                                                                    \
            body                                                                                                       \
        }                                                                                                              \
    }

#endif
