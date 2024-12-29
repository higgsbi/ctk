#include "ctk/collections/vector.h"
#include "ctk/io.h"
#include "ctk/types/string.h"

#define DEFAULT_SIZE 5
TYPE_VECTOR(i32, I32, i32, int_clone, int_free, int_compare)                    // NOLINT
TYPE_VECTOR(String, String, string, string_clone, string_free, string_compare)  // NOLINT

i32 compare_reverse_int(const i32** first, const i32** second) {
    return int_compare(second, first);
}

i32 compare_reverse_string(const String** first, const String** second) {
    return -string_compare(second, first);
}

int main() {
    VecI32* ints = vec_i32_new(DEFAULT_SIZE);
    VecString* strings = vec_string_new(DEFAULT_SIZE);

    for (usize i = 0; i < DEFAULT_SIZE * 2; i++) {
        vec_i32_push_back_literal(ints, i * 2);
        vec_string_push_back(strings, string_from_format(&str("%d index"), i, &str("Index")));
    }

    OptionString firstString = vec_string_pop_first(strings);
    OptionI32 firstInt = vec_i32_pop_first(ints);
    if (option_string_is_present(firstString)) {
        string_free(option_string_get(firstString));
    }
    if (option_i32_is_present(firstInt)) {
        int_free(option_i32_get(firstInt));
    }

    vec_string_delete(strings, 4);
    vec_i32_delete(ints, 4);
    vec_i32_sort(ints);
    vec_string_sort(strings);

    for (usize i = 0; i < ints->count; i++) {
        OptionI32 optInt = vec_i32_get(ints, i);
        OptionString optStr = vec_string_get(strings, i);
        if (option_i32_is_present(optInt) && option_string_is_present(optStr)) {
            print(&str("(ints: %d) and (strings: %S)\n"), *option_i32_get(optInt), option_string_get(optStr));
        }
    }

    vec_i32_sort_custom(ints, compare_reverse_int);
    vec_string_sort_custom(strings, compare_reverse_string);
    print(&str("\n%r\n"), "Sorted");

    FOREACH_VECTOR(i32 integer, ints, { print(&str("%d\n"), integer); });
    FOREACH_VECTOR(String string, strings, { print(&str("%S\n"), &string); });

    vec_i32_free(ints);
    vec_string_free(strings);

    return 0;
}
