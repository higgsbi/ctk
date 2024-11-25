#include "io.h"
#include <stdarg.h>

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
