#ifndef CTK_LIST_H
#define CTK_LIST_H

#define TYPE_LIST(type, type_name, func_name, clone, destroy)                                             \
    typedef struct Node##type_name {                                                                      \
        type* value;                                                                                      \
        struct Node##type_name* next;                                                                     \
    } Node##type_name;                                                                                    \
                                                                                                          \
    typedef struct Node##type_name Node##type_name;                                                       \
                                                                                                          \
    typedef struct {                                                                                      \
        Node##type_name* head;                                                                            \
    } List##type_name;                                                                                    \
                                                                                                          \
    OPTION_TYPE(Node##type_name*, Node##type_name, node_##func_name, null);                               \
                                                                                                          \
    typedef type* (*_clone_##func_name)(type*);                                                           \
    typedef void(_destroy_##func_name)(type*);                                                            \
                                                                                                          \
    static Node##type_name* list_##func_name##_begin(List##type_name* self) {                             \
        return self->head;                                                                                \
    }                                                                                                     \
    static bool list_##func_name##_has_next(Node##type_name* self) {                                      \
        return self != null;                                                                              \
    }                                                                                                     \
    static Node##type_name* list_##func_name##_next(Node##type_name* self) {                              \
        return self->next;                                                                                \
    }                                                                                                     \
    static List##type_name* list_##func_name##_new() {                                                    \
        List##type_name* list = allocate(sizeof(List##type_name), 1);                                     \
        list->head = null;                                                                                \
        return list;                                                                                      \
    }                                                                                                     \
    static void list_##func_name##_free(List##type_name* self) {                                          \
        ASSERT_NONNULL(self);                                                                             \
        Node##type_name* iterator = self->head;                                                           \
        while (iterator != null) {                                                                        \
            destroy(iterator->value);                                                                     \
            Node##type_name* temp = iterator;                                                             \
            iterator = iterator->next;                                                                    \
            free(temp);                                                                                   \
        }                                                                                                 \
        free(self);                                                                                       \
    }                                                                                                     \
    static void list_##func_name##_node_free(Node##type_name* self) {                                     \
        ASSERT_NONNULL(self);                                                                             \
        destroy(self->value);                                                                             \
        free(self);                                                                                       \
    }                                                                                                     \
    static void list_##func_name##_push_back(List##type_name* self, type* element) {                      \
        ASSERT_NONNULL(self);                                                                             \
        if (self->head == null) {                                                                         \
            self->head = (Node##type_name*) allocate(sizeof(Node##type_name), 1);                         \
            self->head->value = element;                                                                  \
            self->head->next = null;                                                                      \
            return;                                                                                       \
        }                                                                                                 \
        Node##type_name* iterator = self->head;                                                           \
        while (iterator->next != null) {                                                                  \
            iterator = iterator->next;                                                                    \
        }                                                                                                 \
        iterator->next = (Node##type_name*) allocate(sizeof(Node##type_name), 1);                         \
        iterator->next->value = element;                                                                  \
        iterator->next->next = null;                                                                      \
    }                                                                                                     \
    static void list_##func_name##_push_back_literal(List##type_name* self, type element) {               \
        ASSERT_NONNULL(self);                                                                             \
        list_##func_name##_push_back(self, clone(&element));                                              \
    }                                                                                                     \
    static OptionNode##type_name list_##func_name##_peek_back(List##type_name* self) {                    \
        ASSERT_NONNULL(self);                                                                             \
        if (self->head == null) {                                                                         \
            return option_node_##func_name##_empty();                                                     \
        }                                                                                                 \
        Node##type_name* iterator = self->head;                                                           \
        while (iterator->next != null) {                                                                  \
            iterator = iterator->next;                                                                    \
        }                                                                                                 \
        return option_node_##func_name(iterator);                                                         \
    }                                                                                                     \
    static OptionNode##type_name list_##func_name##_pop_back(List##type_name* self) {                     \
        ASSERT_NONNULL(self);                                                                             \
        if (self->head == null) {                                                                         \
            return option_node_##func_name##_empty();                                                     \
        }                                                                                                 \
        if (self->head->next == null) {                                                                   \
            Node##type_name* head = self->head;                                                           \
            self->head = null;                                                                            \
            return option_node_##func_name(head);                                                         \
        }                                                                                                 \
        Node##type_name* previous = null;                                                                 \
        Node##type_name* iterator = self->head;                                                           \
        while (iterator->next != null) {                                                                  \
            previous = iterator;                                                                          \
            iterator = iterator->next;                                                                    \
        }                                                                                                 \
        if (previous != null) {                                                                           \
            previous->next = null;                                                                        \
        }                                                                                                 \
        return option_node_##func_name(iterator);                                                         \
    }                                                                                                     \
    static OptionNode##type_name list_##func_name##_pop_head(List##type_name* self) {                     \
        ASSERT_NONNULL(self);                                                                             \
        if (self->head == null) {                                                                         \
            return option_node_##func_name##_empty();                                                     \
        }                                                                                                 \
        Node##type_name* head = self->head;                                                               \
        if (self->head->next == null) {                                                                   \
            self->head = null;                                                                            \
        } else {                                                                                          \
            self->head = self->head->next;                                                                \
        }                                                                                                 \
        return option_node_##func_name(head);                                                             \
    }                                                                                                     \
    static OptionNode##type_name list_##func_name##_pop_index(List##type_name* self, usize entry_index) { \
        ASSERT_NONNULL(self);                                                                             \
        if (self->head == null) {                                                                         \
            return option_node_##func_name##_empty();                                                     \
        }                                                                                                 \
        if (entry_index == 0) {                                                                           \
            return list_##func_name##_pop_head(self);                                                     \
        }                                                                                                 \
        usize i = 0;                                                                                      \
        Node##type_name* previous = null;                                                                 \
        Node##type_name* iterator = self->head;                                                           \
        while (iterator->next != null) {                                                                  \
            previous = iterator;                                                                          \
            iterator = iterator->next;                                                                    \
            if (i == entry_index) {                                                                       \
                break;                                                                                    \
            }                                                                                             \
            i++;                                                                                          \
        }                                                                                                 \
        if (i != entry_index) {                                                                           \
            return option_node_##func_name##_empty();                                                     \
        }                                                                                                 \
        if (previous != null) {                                                                           \
            previous->next = iterator->next;                                                              \
        }                                                                                                 \
        iterator->next = null;                                                                            \
        return option_node_##func_name(iterator);                                                         \
    }                                                                                                     \
    static void list_##func_name##_delete_head(List##type_name* self) {                                   \
        ASSERT_NONNULL(self);                                                                             \
        if (self->head == null) {                                                                         \
            return;                                                                                       \
        }                                                                                                 \
        OptionNode##type_name popped = list_##func_name##_pop_head(self);                                 \
        if (option_node_##func_name##_is_present(popped)) {                                               \
            destroy(popped.value->value);                                                                 \
            free(popped.value);                                                                           \
        }                                                                                                 \
    }                                                                                                     \
    static void list_##func_name##_delete_back(List##type_name* self) {                                   \
        ASSERT_NONNULL(self);                                                                             \
        if (self->head == null) {                                                                         \
            return;                                                                                       \
        }                                                                                                 \
        OptionNode##type_name popped = list_##func_name##_pop_back(self);                                 \
        if (option_node_##func_name##_is_present(popped)) {                                               \
            destroy(popped.value->value);                                                                 \
            free(popped.value);                                                                           \
        }                                                                                                 \
    }                                                                                                     \
    static void list_##func_name##_delete_index(List##type_name* self, usize index) {                     \
        ASSERT_NONNULL(self);                                                                             \
        if (self->head == null) {                                                                         \
            return;                                                                                       \
        }                                                                                                 \
        OptionNode##type_name popped = list_##func_name##_pop_index(self, index);                         \
        if (option_node_##func_name##_is_present(popped)) {                                               \
            destroy(popped.value->value);                                                                 \
            free(popped.value);                                                                           \
        }                                                                                                 \
    }                                                                                                     \
    static List##type_name* list_##func_name##_clone(List##type_name* self) {                             \
        ASSERT_NONNULL(self);                                                                             \
        List##type_name* cloned = allocate(sizeof(List##type_name), 1);                                   \
        cloned->head = null;                                                                              \
                                                                                                          \
        Node##type_name* iterator = self->head;                                                           \
        while (iterator != null) {                                                                        \
            list_##func_name##_push_back(cloned, clone(iterator->value));                                 \
            iterator = iterator->next;                                                                    \
        }                                                                                                 \
        return cloned;                                                                                    \
    }

#endif
