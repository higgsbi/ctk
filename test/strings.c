#include "ctk/io.h"
#include "ctk/types/string.h"

int main() {
    // Comparison
    String* string = string_from_str(&str("Hello World!"));
    print(&str("Original: %S, Slice: %s\n"), string, string_slice(string));
    assert(str_equals_str(&str("Hello World!"), string_slice(string)));

    // Format
    String* formatted = string_from_format(&str("Our number is %d %d"), 2, 2);
    print(&str("Formatted: '%S', Desired: '%s'\n"), formatted, &str("Our number is 2 2"));
    assert(string_equals_str(formatted, &str("Our number is 2 2")));

    // Push back
    string_push_str(string, &str(" -> Easy as "));
    string_push_char(string, 'A');
    string_push_char(string, 'B');
    string_push_char(string, 'C');
    string_push_str(string, &str(", simple as doe rae me"));
    print(&str("Appended: %s\n"), string_slice(string));
    assert(string_equals_chars(string, "Hello World! -> Easy as ABC, simple as doe rae me"));

    // Index
    Str url = str("duckduckgo.com");
    OptionIndex index_dot = str_index_first_char(&url, '.');
    Str extension = str_subslice(&url, index_dot.value + 1, url.length - index_dot.value - 1);
    print(&str("Extension of %s is %s\n"), &url, &extension);
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
    print(&str("Before Trim: '%S'\n"), spaced_out);
    string_trim(spaced_out);
    print(&str("After Trim: '%S'\n"), spaced_out);
    assert(string_equals_str(spaced_out, &str("Spaced Message")));

    // Split
    Str split_string = str("One/Two/Three");
    print(&str("Split Whole: '%s'\n"), &split_string);

    VecStrSlice* split_slice = str_split_slice(&split_string, '/');
    assert(split_slice->count == 3);
    FOREACH_VECTOR(Str str, split_slice, { print(&str("Split: %s\n"), &str); });

    Str split_string_b = str("/One//Two//Three/");
    VecStringOwned* split = str_split(&split_string_b, '/');
    assert(split->count == 3);

    // Replaced
    String* original = string_from_str(&str("NOOneNOTwoNOThreeNOFourNOFiveNO"));
    print(&str("Before replacing: '%S'\n"), original);
    string_replace(original, &str("NO"), &str(" "));
    print(&str("After replacing: '%S'\n"), original);
    assert(string_equals_str(original, &str(" One Two Three Four Five ")));

    // Free
    string_free(original);
    vec_str_slice_free(split_slice);
    vec_string_owned_free(split);
    string_free(spaced_out);
    string_free(formatted);
    string_free(string);

    return 0;
}
