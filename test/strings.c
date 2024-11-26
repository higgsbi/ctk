#include "ctk/io.h"
#include "ctk/types/string.h"

int main() {
    // Comparison
    String* string = string_from_str(&str("Hello World!"));
    ctk_printf("Original: %S, Splice: %s\n", string, string_splice(string));
    assert(str_equals_str(&str("Hello World!"), string_splice(string)));

    // Format
    String* formatted = string_from_format(&str("Our number is %d %d"), 2, 2);
    ctk_printf("Original: '%s', Formatted: '%S'\n", &str("Our number is 2 2"), formatted);
    assert(string_equals_str(formatted, &str("Our number is 2 2")));

    // Push back
    string_push_str(string, &str(" -> Easy as "));
    string_push_char(string, 'A');
    string_push_char(string, 'B');
    string_push_char(string, 'C');
    string_push_str(string, &str(", simple as doe rae me"));
    ctk_printf("Appended: %s\n", string_splice(string));
    assert(string_equals_cstr(string, "Hello World! -> Easy as ABC, simple as doe rae me"));

    // Index
    Str url = str("duckduckgo.com");
    OptionIndex index_dot = str_index_first_char(&url, '.');
    Str extension = str_subsplice(&url, index_dot.value + 1, url.length - index_dot.value - 1);
    ctk_printf("Extension of %s is %s\n", &url, &extension);
    assert(str_equals_str(&extension, &str("com")));

    // Contains
    assert(str_contains_char(&url, 'k'));
    assert(str_contains_char(&url, 'm'));
    assert(str_contains_str(&url, &str("")));
    assert(str_contains_str(&url, &str(".com")));
    assert(!str_contains_str(&url, &str("short_test")));
    assert(!str_contains_str(&url, &str("long_test_with_many_characters")));
    assert(!str_contains_str(&url, &str(" ")));

    // Trim
    String* spaced_out = string_from_str(&str("  Spaced Message  "));
    ctk_printf("Before Trim: '%S'\n", spaced_out);
    string_trim(spaced_out);
    ctk_printf("After Trim: '%S'\n", spaced_out);
    assert(string_equals_str(spaced_out, &str("Spaced Message")));

    // Split
    Str split_string = str("One/Two/Three");
    ctk_printf("Split Whole: '%s'\n", &split_string);

    VecStr* split_splice = str_split_splice(&split_string, '/');
    assert(split_splice->count == 3);
    FOREACH_VECTOR(Str str, split_splice, { ctk_printf("Split: %s\n", &str); });

    Str split_string_b = str("/One//Two//Three/");
    VecString* split = str_split(&split_string_b, '/');
    assert(split->count == 3);

    // Replaced
    String* original = string_from_str(&str("NOOneNOTwoNOThreeNOFourNOFiveNO"));
    ctk_printf("Before replacing: '%S'\n", original);
    string_replace(original, &str("NO"), &str(" "));
    ctk_printf("After replacing: '%S'\n", original);
    assert(string_equals_str(original, &str(" One Two Three Four Five ")));

    // Free
    string_free(original);
    vec_str_free(split_splice);
    vec_string_free(split);
    string_free(spaced_out);
    string_free(formatted);
    string_free(string);

    return 0;
}
