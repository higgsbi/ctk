#include "ctk/collections/list.h"
#include "ctk/io.h"

TYPE_LIST(i32, I32, i32, int_clone, int_free)  // NOLINT

int main() {
    ListI32* list = list_i32_new();
    list_i32_push_back_literal(list, 1);
    list_i32_push_back_literal(list, 2);
    list_i32_push_back_literal(list, 3);
    list_i32_push_back_literal(list, 4);

    OptionNodeI32 peek = list_i32_peek_back(list);
    if (option_node_i32_is_present(peek)) {
        print(&str("Peek Back: %d\n"), *peek.value->value);
        assert(*peek.value->value == 4);
    }

    list_i32_delete_back(list);

    OptionNodeI32 popped = list_i32_pop_back(list);
    if (option_node_i32_is_present(popped)) {
        print(&str("Popped Back: %d\n"), *popped.value->value);
        assert(*popped.value->value == 3);
        list_i32_node_free(popped.value);
    }

    for (NodeI32* i = list_i32_begin(list); list_i32_has_next(i); i = list_i32_next(i)) {
        print(&str("%d\n"), *i->value);
    }

    ListI32* cloned = list_i32_clone(list);
    popped = list_i32_pop_index(cloned, 0);
    assert(popped.present);
    list_i32_node_free(popped.value);
    print(&str("Cloned (first removed)\n"));

    for (NodeI32* i = list_i32_begin(cloned); list_i32_has_next(i); i = list_i32_next(i)) {
        print(&str("%d\n"), *i->value);
    }

    list_i32_free(cloned);
    list_i32_free(list);
}
