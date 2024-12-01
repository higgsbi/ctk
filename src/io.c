#include "io.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "types/string.h"
#include "types/types.h"

void _ctk_fprintf(FILE* file, const char* format, va_list args);

void ctk_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    _ctk_fprintf(stdout, format, args);
    va_end(args);
}

void ctk_fprintf(FILE* file, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _ctk_fprintf(file, format, args);
    va_end(args);
}

void ctk_fprint_args(FILE* file, const char* format, va_list args) {
    _ctk_fprintf(file, format, args);
}

Process process_start(ProcessMode mode, Str* command) {
    Process process;
    process.command = string_from_str(command);
    string_push_char(process.command, '\0');

    if (mode == PROCESS_READ) {
        process.os_process = popen(command->buffer, "r");
    } else {
        process.os_process = popen(command->buffer, "w");
    }

    return process;
}

Process process_startf(ProcessMode mode, Str* format, ...) {
    va_list args;
    va_start(args, format);

    Process process;
    process.command = string_from_args(format, args);
    string_push_char(process.command, '\0');

    if (mode == PROCESS_READ) {
        process.os_process = popen(process.command->buffer, "r");
    } else {
        process.os_process = popen(process.command->buffer, "w");
    }

    va_end(args);

    return process;
}

String* process_read_all(Process* self) {
    String* input = string_with_capacity(PROCESS_DEFAULT_INPUT_LENGTH);
    c8 current;

    while ((current = fgetc(self->os_process))) {
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
    ASSERT_NONNULL(self->os_process);
    ASSERT_NONNULL(self->command);

    string_free(self->command);
    pclose(self->os_process);
}

void command_run(String* command) {
    if (command->buffer[command->length - 1] != '\0') {
        string_push_char(command, '\0');
    }

    system(command->buffer);
}

void command_runf(Str* format, ...) {
    va_list args;
    va_start(args, format);

    String* formatted = string_from_args(format, args);
    string_push_char(formatted, '\0');
    system(formatted->buffer);

    string_free(formatted);
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
void _ctk_fprintf(FILE* file, const char* format, va_list args) {
    String* string;
    Str* str;
    char* cstr;
    char character;
    int digit;
    double decimal;
    bool boolean;
    void* address;

    while (*format != '\0') {
        char current = *format;
        char next = *(format + 1);

        if (current == '%' && next != '\0') {
            switch (next) {
                case 'S':
                case 'p':
                    string = va_arg(args, String*);
                    fwrite(string->buffer, sizeof(u8), string->length, file);
                    break;
                case 's':
                    str = va_arg(args, Str*);
                    fwrite(str->buffer, sizeof(u8), str->length, file);
                    break;
                case 'r':
                    cstr = va_arg(args, char*);
                    fprintf(file, "%s", cstr);
                    break;
                case 'd':
                    digit = va_arg(args, i32);
                    fprintf(file, "%d", digit);
                    break;
                case 'f':
                    decimal = va_arg(args, i32);
                    fprintf(file, "%f", decimal);
                    break;
                case 'b':
                    boolean = va_arg(args, i32);
                    if (boolean) {
                        fprintf(file, "%s", "true");
                    } else {
                        fprintf(file, "%s", "false");
                    }
                    break;
                case 'a':
                    address = va_arg(args, void*);
                    fprintf(file, "%p", address);
                    break;
                case 'c':
                    character = va_arg(args, u32);
                    fprintf(file, "%c", character);
                    break;
                case '%':
                    putc('%', file);
                    break;
            }
            format += 2;
        } else {
            putc(current, file);
            format++;
        }
    }
}
