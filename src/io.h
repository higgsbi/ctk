#ifndef CTK_INCLUDE_H
#define CTK_INCLUDE_H

#include <stdio.h>
#include "assert.h"
#include "types/option.h"
#include "types/string.h"

typedef struct {
    FILE* os_process;
    String* command;
} Process;

typedef enum { PROCESS_READ, PROCESS_WRITE } ProcessMode;
#define PROCESS_DEFAULT_INPUT_LENGTH 20

Process process_start(ProcessMode mode, Str* command);
Process process_startf(ProcessMode mode, Str* format, ...);
String* process_read_all(Process* self);
void process_end(Process* self);

void command_run(String* command);
void command_runf(Str* format, ...);
void command_clear();
void command_newline(usize count);

void ctk_printf(const char* format, ...);

/** Types:
 *
 * %S -> String
 * %p -> path
 * %s -> str
 * %r -> raw cstring
 * %i -> int
 * %f -> float
 * %b -> bool
 * %a -> address
 */
void ctk_fprintf(FILE* file, const char* format, ...);
void ctk_fprint_args(FILE* file, const char* format, va_list args);

#endif
