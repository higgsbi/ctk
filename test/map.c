#include "ctk/collections/map.h"
#include "ctk/io.h"
#include "ctk/types/string.h"

TYPE_MAP(Str, Str, str, str_free_deep, str_clone_deep)  // NOLINT

int main() {
    MapStr* map = map_str_new(4);
    map_str_insert_owned(map, str_owned("Hello"), str_owned("World"));
    map_str_insert_owned(map, str_owned("Hi"), str_owned("Planet"));
    map_str_insert_owned(map, str_owned("Hey there"), str_owned("Big Blue Guy"));
    map_str_insert_literal(map, str_owned("Greetings"), str("Terrestrial Environmnent"));
    map_str_insert_owned(map, str_owned("Other"), str_owned("Why"));
    OptionEntryStr option = map_str_get(map, &str("Hello"));

    if (option_entry_str_is_present(option)) {
        EntryStr* entry = option.value;
        print(&str("Retrieved: (%s -> %s)\n"), entry->key, entry->value);
    }

    option = map_str_get(map, &str("BadKey"));
    if (option_entry_str_is_empty(option)) {
        print(&str("No entry by the key ('%s') could be found\n"), &str("BadKey"));
    }

    EntryStr* unsafe = map_str_get(map, &str("Greetings")).value;
    print(&str("Retrieved without Optional: (%s -> %s)\n"), unsafe->key, unsafe->value);

    print(&str("\nSlice All:\n"));
    MapSliceStr slice = map_slice_str_new(map);
    for (usize i = 0; i < slice.count; i++) {
        EntryStr* entry = slice.entries->elements[i];
        print(&str("(%s -> %s)\n"), entry->key, entry->value);
    }

    OptionEntryStr removed = map_str_remove(map, &str("Hi"));
    assert(removed.present);
    entry_str_free(removed.value);
    assert(map_str_delete(map, &str("Hello")));

    assert(!map_str_remove(map, &str("Whatever")).present);

    print(&str("\nForEach Macro ('Hi' + 'Hello' Removed):\n"));
    FOREACH_MAP(EntryStr entry, map, { print(&str("(%s -> %s)\n"), entry.key, entry.value); });

    map_slice_str_free(&slice);
    map_str_free(map);

    return 0;
}
