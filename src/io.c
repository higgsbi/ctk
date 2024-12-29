#include "io.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "math/aabb.h"
#include "types/string.h"
#include "types/types.h"

void _fprint(FILE* file, const Str* format, va_list args);

Process* process_start(ProcessMode mode, const CStr* command) {
    if (mode == PROCESS_READ) {
        return popen(command->buffer, "r");
    }
    return popen(command->buffer, "w");
}

Process* process_startf(ProcessMode mode, const Str* format, ...) {
    va_list args;
    va_start(args, format);

    CString* command = cstring_from_args(format, args);
    Process* process;

    if (mode == PROCESS_READ) {
        process = popen(command->buffer, "r");

    } else {
        process = popen(command->buffer, "w");
    }

    va_end(args);
    cstring_free(command);

    return process;
}

String* process_read_all(Process* self) {
    String* input = string_with_capacity(PROCESS_DEFAULT_INPUT_LENGTH);
    c8 current;

    while ((current = fgetc(self))) {
        if (current == '\0' || current == -1) {
            break;
        }

        string_push_char(input, current);
    }

    if (input->buffer[input->length - 1] == '\n') {
        input->buffer[input->length - 1] = '\0';
        input->length--;
    }

    return input;
}

void process_end(Process* self) {
    ASSERT_NONNULL(self);
    pclose(self);
}

void command_run(const CStr* command) {
    system(command->buffer);
}

void command_runf(const Str* format, ...) {
    va_list args;
    va_start(args, format);

    CString* formatted = cstring_from_args(format, args);
    system(formatted->buffer);

    cstring_free(formatted);
    va_end(args);
}

void command_clear() {
    system("clear");
}

#define NEW_LINE "echo &&"
#define NEW_LINE_SIZE (sizeof("echo &&") - 1)
// Avoid stack overflow
#define MAX_NEW_LINE 100

void command_newline(usize count) {
    // Concatenating string avoids excess expensive system calls
    char command[(NEW_LINE_SIZE * count) + 1];
    command[0] = '\0';

    for (usize i = 0; i < count && i < MAX_NEW_LINE; i++) {
        strcat(command, NEW_LINE);
    }

    command[(NEW_LINE_SIZE * count) - 3] = '\0';
    system(command);
}

void print(const Str* format, ...) {
    va_list args;
    va_start(args, format);
    _fprint(stdout, format, args);
    va_end(args);
}

void fprint(FILE* file, const Str* format, ...) {
    va_list args;
    va_start(args, format);
    _fprint(file, format, args);
    va_end(args);
}

void fprint_args(FILE* file, const Str* format, va_list args) {
    _fprint(file, format, args);
}

/* Types:
 *
 * %S -> String
 * %s -> str
 * %r -> raw cstring
 * %c -> char
 * %d -> digit
 * %f -> float
 * %b -> bool
 * %a -> address
 */
void _fprint(FILE* file, const Str* format, va_list args) {
    const c8* buffer = format->buffer;
    String* string;
    Str* str;
    Vec3* vec;
    AABB* aabb;

    for (usize i = 0; i < format->length; i++, buffer++) {
        char current = *buffer;

        if (current != '%' || (i >= format->length - 1)) {
            putc(current, file);
        } else {
            switch (*(buffer + 1)) {
                case 'S':
                case 'p':
                    string = va_arg(args, String*);
                    fwrite(string->buffer, sizeof(c8), string->length, file);
                    break;
                case 's':
                    str = va_arg(args, Str*);
                    fwrite(str->buffer, sizeof(c8), str->length, file);
                    break;
                case 'r':
                    fprintf(file, "%s", va_arg(args, char*));
                    break;
                case 'd':
                    fprintf(file, "%d", va_arg(args, i32));
                    break;
                case 'f':
                    fprintf(file, "%f", va_arg(args, f64));
                    break;
                case 'b':
                    if (va_arg(args, i32)) {
                        fprintf(file, "%s", "true");
                    } else {
                        fprintf(file, "%s", "false");
                    }
                    break;
                case 'a':
                    fprintf(file, "%p", va_arg(args, void*));
                    break;
                case 'c':
                    fprintf(file, "%c", va_arg(args, u32));
                    break;
                case 'v':
                    vec = va_arg(args, Vec3*);
                    fprintf(file, "[%f, %f, %f]", vec->x, vec->y, vec->z);
                    break;
                case 'B':
                    aabb = va_arg(args, AABB*);
                    fprintf(file, "([%f, %f, %f], [%f, %f, %f])", aabb->lower.x, aabb->lower.y, aabb->lower.z,
                            aabb->higher.x, aabb->higher.y, aabb->higher.z);
                    break;
                case '%':
                    putc('%', file);
                    break;
            }
            i++;
            buffer++;
        }
    }
}
