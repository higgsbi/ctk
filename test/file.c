#include "ctk/file/file.h"
#include "ctk/file/path.h"
#include "ctk/io.h"
#include "ctk/types/string.h"

int main() {
    Path* path = path_current();
    path_resolve(path, &str("sample.txt"));

    File file = file_from_path(path);
    file_open(&file, FILE_OVERWRITE);

    file_write(&file, &str("%s: %d\n"), &str("Line"), 1);
    file_write(&file, &str("%r: %d\n"), "Line", 2);
    file_write_str(&file, &str("Line: 3\n"));
    file_write_line(&file, &str(""));
    file_write_line(&file, &str("Line: 4"));

    file_reset(&file);

    String* line = string_empty();

    for (usize i = 0; file_read_line(&file, line); i++) {
        print(&str("'%S'\n"), line);
        assert(i != 0 || string_equals_str(line, &str("Line: 1")));
        assert(i != 1 || string_equals_str(line, &str("Line: 2")));
        assert(i != 2 || string_equals_str(line, &str("Line: 3")));
        assert(i != 3 || string_equals_str(line, &str("")));
        assert(i != 4 || string_equals_str(line, &str("Line: 4")));
    }

    string_free(line);
    file_free(&file);

    return 0;
}
