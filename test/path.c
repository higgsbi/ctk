#include "ctk/file/path.h"
#include "ctk/io.h"
#include "ctk/types/string.h"

int main() {
    Path* path = path_root();
    assert(string_equals_str(path, &str("/")));
    print(&str("'%p'\n"), path);

    path_resolve(path, &str("usr/share/applications"));
    print(&str("'%p'\n"), path);
    assert(string_equals_str(path, &str("/usr/share/applications/")));

    Path* helix = path_clone(path);
    path_resolve(helix, &str("Helix.desktop"));
    print(&str("'%p'\n"), helix);

    Str extension = path_extension_slice(helix);
    assert(str_equals_str(&extension, &str("desktop")));
    print(&str("'%S' extension is '%s'\n"), helix, &extension);

    Path* home = path_user();
    assert(path_exists(home));
    assert(path_is_directory(home));

    print(&str("'%p' exists!\n'%p' is a directory: %b\n"), home, home, path_is_directory(home));

    Path* blank_current = path_from_string(string_from_str(&str("")));
    print(&str("'%p' was created with input: \"\"\n"), blank_current);

    Path* child = path_from_str(&str("/home/nate/test/"));
    path_to_parent(child);
    print(&str("Parent of '%s' is '%p'\n"), &str("/home/nate/test/"), child);
    assert(string_equals_str(child, &str("/home/nate/")));

    Path* relative = path_from_str(&str("~/Documents/../Pictures/Screenshots/"));
    print(&str("'%p' before normalization\n"), relative);
    path_normalize(relative);
    print(&str("'%p' after normalization\n"), relative);

    path_free(relative);
    path_free(child);
    path_free(blank_current);
    path_free(home);
    path_free(helix);
    path_free(path);

    return 0;
}
