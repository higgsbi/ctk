#ifndef CTK_INCLUDE_H
#define CTK_INCLUDE_H

#include <stdio.h>
#include "assert.h"
#include "types/option.h"
#include "types/string.h"

typedef FILE Process;

typedef enum { PROCESS_READ, PROCESS_WRITE } ProcessMode;
#define PROCESS_DEFAULT_INPUT_LENGTH 20

Process* process_start(ProcessMode mode, const CStr* command);
Process* process_startf(ProcessMode mode, const Str* format, ...);
String* process_read_all(Process* self);
void process_end(Process* self);

void command_run(const CStr* command);
void command_runf(const Str* format, ...);
void command_clear();
void command_newline(usize count);

void print(const Str* format, ...);

/** Types:
 *
 * %S -> String*
 * %p -> path*
 * %s -> str*
 * %r -> raw cstring
 * %i -> int
 * %f -> float
 * %b -> bool
 * %a -> address
 */
void fprint(FILE* file, const Str* format, ...);
void fprint_args(FILE* file, const Str* format, va_list args);

#endif
