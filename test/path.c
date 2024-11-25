#include "ctk/file/path.h"
#include <assert.h>
#include "ctk/io.h"
#include "ctk/types/string.h"

int main() {
    Path* path = path_root();
    assert(string_equals_str(path, &str("/")));
    ctk_printf("'%p'\n", path);

    path_resolve(path, &str("usr/share/applications"));
    ctk_printf("'%p'\n", path);
    assert(string_equals_str(path, &str("/usr/share/applications/")));

    Path* helix = path_clone(path);
    path_resolve(helix, &str("Helix.desktop"));
    ctk_printf("'%p'\n", helix);

    Str extension = path_extension_splice(helix);
    assert(str_equals_str(&extension, &str("desktop")));
    ctk_printf("'%S' extension is '%s'\n", helix, &extension);

    Path* home = path_user();
    assert(path_exists(home));
    assert(path_is_directory(home));

    ctk_printf("'%p' exists!\n'%p' is a directory: %b\n", home, home, path_is_directory(home));

    Path* blank_current = path_from_string(string_from_str(&str("")));
    ctk_printf("'%p' was created with input: \"\"\n", blank_current);

    Path* child = path_from_str(&str("/home/nate/test/"));
    path_to_parent(child);
    ctk_printf("Parent of '%s' is '%p'\n", &str("/home/nate/test/"), child);
    assert(string_equals_str(child, &str("/home/nate/test/")));

    Path* relative = path_from_str(&str("~/Documents/../Pictures/Screenshots/"));
    ctk_printf("'%p' before normalization\n", relative);
    path_normalize(relative);
    ctk_printf("'%p' after normalization\n", relative);

    path_free(relative);
    path_free(child);
    path_free(blank_current);
    path_free(home);
    path_free(helix);
    path_free(path);

    return 0;
}
