#include "ctk/file/path.h"
#include "ctk/io.h"
#include "ctk/types/string.h"

int main() {
    // COMMAND

    Path* dir = path_root();
    String* command = string_from_format(&str("ls -%r %p"), "Al", dir);

    command_clear();
    ctk_printf("%s", &str("\n'ls -Al' command output:\n\n"));
    command_run(command);
    ctk_printf("%s", &str("\n\n'ls -A' command output:\n\n"));
    command_runf(&str("ls -%r %p"), "A", dir);
    string_free(command);
    path_free(dir);

    // PROCESS

    Process process = process_start(PROCESS_READ, &str("pwd"));
    String* input = process_read_all(&process);
    ctk_printf("\nProcess: '%S' \n", input);
    process_end(&process);
    string_free(input);

    // PROCESS FORMATTED

    process = process_startf(PROCESS_READ, &str("pwd | tr '%c' '%c'"), '/', '-');
    input = process_read_all(&process);
    ctk_printf("\nFormatted Process: '%S'\n", input);
    process_end(&process);
    string_free(input);

    return 0;
}
