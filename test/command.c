#include "ctk/file/path.h"
#include "ctk/io.h"
#include "ctk/types/string.h"

int main() {
    // COMMAND

    Path* dir = path_root();
    CString* command = cstring_from_format(&str("ls -%r %p"), "Al", dir);

    command_clear();

    // PRE-FORMATTING

    print(&str("%s"), &str("\n'ls -Al' command output:\n\n"));
    command_run(cstring_slice(command));
    cstring_free(command);

    // INLINE FORMATTING

    print(&str("%s"), &str("\n\n'ls -A' command output:\n\n"));
    command_runf(&str("ls -%r %p"), "A", dir);
    path_free(dir);

    // PROCESS

    Process* process = process_start(PROCESS_READ, &cstr("pwd"));
    String* input = process_read_all(process);
    print(&str("\nProcess: '%S' \n"), input);
    process_end(process);
    string_free(input);

    // PROCESS FORMATTED

    process = process_startf(PROCESS_READ, &str("pwd | tr '%c' '%c'"), '/', '-');
    input = process_read_all(process);
    print(&str("\nFormatted Process: '%S'\n"), input);
    process_end(process);
    string_free(input);

    return 0;
}
