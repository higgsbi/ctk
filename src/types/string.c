#include "string.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>  // NOLINT
#include "types.h"

#define ADDRESS_DIGITS_64 18
#define I32_DIGITS 12
#define F32_DIGITS 32

// LIFECYCLE

Str str_from_cstr(const char* cstr) {
    return (Str) {.buffer = cstr, .length = strlen(cstr)};
}

Str str_from_char_array(const char* char_array, size_t length) {
    return (Str) {.buffer = char_array, .length = length};
}

Str* string_splice(const String* self) {
    return (Str*) self;
}

Str str_subsplice(const Str* self, usize start, usize length) {
    ASSERT_NONNULL(self);
    ASSERT_IN_RANGE(start + length - 1, self->length);

    return (Str) {
        .buffer = self->buffer + start,
        .length = length,
    };
}

String* string_empty() {
    return string_with_capacity(STRING_DEFAULT_LENGTH);
}

String* string_with_capacity(usize capacity) {
    String* string = allocate(sizeof(String), 1);
    string->capacity = capacity;
    string->length = 0;
    string->buffer = allocate(sizeof(c8), string->capacity);

    return string;
}

String* string_from_format(const Str* format, ...) {
    va_list args;
    va_start(args, format);

    String* self = allocate(sizeof(String), 1);
    self->capacity = format->length;
    self->length = 0;
    self->buffer = allocate(sizeof(char), self->capacity);
    const c8* buffer = format->buffer;

    for (usize i = 0; i < format->length; i++) {
        char current = *buffer;
        char next = *(buffer + 1);
        String* string;
        Str* str;
        char* cstr;
        char character;
        i32 digit;
        f32 decimal;
        bool boolean;
        void* address;

        if (current == '%' && next != '\0') {
            switch (next) {
                case 'S':
                    string = va_arg(args, String*);
                    string_push_str(self, string_splice(string));
                    break;
                case 's':
                    str = va_arg(args, Str*);
                    string_push_str(self, str);
                    break;
                case 'r':
                    cstr = va_arg(args, char*);
                    Str str_convert = str_from_cstr(cstr);
                    string_push_str(self, &str_convert);
                    break;
                case 'd':
                    digit = va_arg(args, i32);
                    char digit_buffer[I32_DIGITS];
                    snprintf(digit_buffer, I32_DIGITS, "%d", digit);
                    string_push_cstr(self, digit_buffer);
                    break;
                case 'f':
                    decimal = va_arg(args, i32);
                    char float_buffer[F32_DIGITS];
                    snprintf(float_buffer, F32_DIGITS, "%f", decimal);
                    string_push_cstr(self, float_buffer);
                    break;
                case 'b':
                    boolean = va_arg(args, i32);
                    if (boolean) {
                        string_push_cstr(self, "true");
                    } else {
                        string_push_cstr(self, "false");
                    }
                    break;
                case 'a':
                    address = va_arg(args, void*);
                    char buffer[ADDRESS_DIGITS_64];
                    snprintf(buffer, ADDRESS_DIGITS_64, "%p", address);
                    string_push_cstr(self, buffer);
                    break;
                case 'c':
                    character = va_arg(args, u32);
                    string_push_char(self, character);
                    break;
                case '%':
                    string_push_char(self, '%');
                    break;
            }
            buffer += 2;
        } else {
            string_push_char(self, current);
            buffer++;
        }
    }

    va_end(args);

    return self;
}

String* string_from_str(const Str* str) {
    String* string = allocate(sizeof(String), 1);
    string->capacity = str->length;
    string->length = str->length;
    string->buffer = allocate(sizeof(c8), string->capacity);
    strncpy(string->buffer, str->buffer, string->capacity);

    return string;
}

String* string_clone(const String* self) {
    String* string = allocate(sizeof(String), 1);
    string->capacity = self->length;
    string->length = self->length;
    string->buffer = allocate(sizeof(c8), string->capacity);
    strncpy(string->buffer, self->buffer, string->capacity);

    return string;
}

Str* str_clone(const Str* self) {
    Str* clone = allocate(sizeof(Str), 1);
    clone->buffer = self->buffer;
    clone->length = self->length;

    return clone;
}

void string_free(String* self) {
    free(self->buffer);
    free(self);
}

void str_free(Str* self) {
    free(self);
}

// MUTATION

void string_trim(String* self) {
    usize start = 0;
    usize length = self->length;

    for (usize i = 0; i < self->length; i++) {
        if (self->buffer[i] == ' ') {
            start++;
        } else {
            break;
        }
    }

    for (usize i = self->length - 1; i > start; i--) {
        if (self->buffer[i] == ' ') {
            length--;
        } else {
            break;
        }
    }

    self->length = length - start;
    memcpy(self->buffer, self->buffer + start, self->length);
}

void string_push_cstr(String* self, const char* cstr) {
    Str converted = str_from_cstr(cstr);
    string_push_str(self, &converted);
}

void string_push_str(String* self, const Str* str) {
    usize length = str->length;

    if (self->capacity <= (self->length + length)) {
        if ((self->capacity * 2) < (self->capacity + length)) {
            self->capacity = self->capacity + length;
        } else {
            self->capacity = self->capacity * 2;
        }
        self->buffer = reallocate(self->buffer, self->capacity);
    }

    strncpy(self->buffer + self->length, str->buffer, length);
    self->length += length;
}

void string_push_char(String* self, c8 character) {
    if (self->capacity <= self->length) {
        self->capacity = self->capacity * 2;
        self->buffer = reallocate(self->buffer, self->capacity);
    }
    self->buffer[self->length] = character;
    self->length++;
}

void string_replace(String* self, Str* replaced, Str* replacer) {
    if (replaced->length <= 0 || self->length < replaced->length) {
        return;
    }

    String* new_string = string_with_capacity(self->length);

    for (usize i = 0; i < self->length; i++) {
        usize last_match = 0;
        for (usize j = 0; j < replaced->length; j++) {
            if (i + j >= self->length || (self->buffer[i + j] != replaced->buffer[j])) {
                // Not matching -> push non-replaced existing string
                usize added_index = 0;
                do {
                    string_push_char(new_string, self->buffer[i + added_index]);
                    added_index++;
                } while (added_index < last_match);

                break;
            }

            if (j >= replaced->length - 1) {
                // Full match -> push replacer
                string_push_str(new_string, replacer);
                i += replaced->length - 1;
            }

            // One character matched -> not necessarily full match, keep track
            last_match++;
        }
    }

    // Put new onto old
    free(self->buffer);
    self->buffer = new_string->buffer;
    self->length = new_string->length;
    self->capacity = new_string->capacity;
    free(new_string);
}

// QUERY

bool str_contains_str(const Str* self, const Str* target) {
    if (self->length < target->length) {
        return false;
    }

    if (target->length == 0) {
        return true;
    }

    for (usize i = 0; i < self->length - target->length + 1; i++) {
        for (usize j = 0; j < target->length; j++) {
            char char_at_self = self->buffer[i + j];
            char char_at_target = target->buffer[j];
            if (char_at_self != char_at_target) {
                break;
            }
            if (j == target->length - 1) {
                return true;
            }
        }
    }

    return false;
}

// if string does not contain delimeter, return empty vector
VecStr* str_split_splice(const Str* self, char delimiter) {
    VecStr* split = vec_str_new(2);
    i32 last_hit_index = -1;

    for (usize i = 0; i < self->length; i++) {
        char char_at = self->buffer[i];

        if (delimiter == char_at) {
            if (i - last_hit_index > 1) {
                Str subsplice = str_subsplice(self, last_hit_index + 1, i - last_hit_index - 1);
                vec_str_push_back(split, str_clone(&subsplice));
            }
            last_hit_index = i;
        } else if (i >= self->length - 1 && last_hit_index >= 0) {
            Str subsplice = str_subsplice(self, last_hit_index + 1, i - last_hit_index);
            vec_str_push_back(split, str_clone(&subsplice));
        }
    }

    return split;
}

VecString* str_split(const Str* self, char delimiter) {
    VecString* split = vec_string_new(2);
    i32 last_hit_index = -1;

    for (usize i = 0; i < self->length; i++) {
        char char_at = self->buffer[i];

        if (delimiter == char_at) {
            if (i - last_hit_index > 1) {
                Str subsplice = str_subsplice(self, last_hit_index + 1, i - last_hit_index - 1);
                vec_string_push_back(split, string_from_str(&subsplice));
            }

            last_hit_index = i;
        } else if (i >= self->length - 1 && last_hit_index >= 0) {
            Str subsplice = str_subsplice(self, last_hit_index + 1, i - last_hit_index);
            vec_string_push_back(split, string_from_str(&subsplice));
        }
    }

    return split;
}

bool str_contains_char(const Str* self, char target) {
    for (usize i = 0; i < self->length; i++) {
        if (self->buffer[i] == target) {
            return true;
        }
    }

    return false;
}

// INDEX

OptionIndex string_index_first_n_char(String* self, c8 character, usize n) {
    usize n_index = 0;

    for (usize i = 0; i < self->length; i++) {
        u8 char_at = self->buffer[i];
        if (character == char_at) {
            n_index++;
            if (n_index >= n) {
                return option_index(i);
            }
        }
    }

    return option_index_empty();
}

OptionIndex string_index_last_n_char(String* self, c8 character, usize n) {
    usize n_index = 0;

    for (i32 i = self->length - 1; i >= 0; i--) {
        u8 char_at = self->buffer[i];
        if (character == char_at) {
            n_index++;
            if (n_index >= n) {
                return option_index(i);
            }
        }
    }

    return option_index_empty();
}

OptionIndex str_index_first_char(const Str* self, c8 character) {
    for (usize i = 0; i < self->length; i++) {
        u8 char_at = self->buffer[i];
        if (character == char_at) {
            return option_index(i);
        }
    }

    return option_index_empty();
}

OptionIndex str_index_last_char(const Str* self, c8 character) {
    for (i32 i = self->length - 1; i >= 0; i--) {
        u8 char_at = self->buffer[i];
        if (character == char_at) {
            return option_index(i);
        }
    }

    return option_index_empty();
}

// COMPARISON

bool string_equals_string(const String* self, const String* other) {
    usize length = self->length < other->length ? self->length : other->length;
    return memcmp(self->buffer, other->buffer, length) == 0;
}

bool string_equals_str(const String* self, const Str* other) {
    usize length = self->length < other->length ? self->length : other->length;
    return memcmp(self->buffer, other->buffer, length) == 0;
}

bool string_equals_cstr(const String* self, const u8* other) {
    usize length_other = strlen((const char*) other);
    usize length = self->length < length_other ? self->length : length_other;
    return memcmp(self->buffer, other, length) == 0;
}

bool str_equals_str(const Str* self, const Str* other) {
    usize length = self->length < other->length ? self->length : other->length;
    return memcmp(self->buffer, other->buffer, length) == 0;
}

bool str_equals_cstr(const Str* self, const u8* other) {
    usize length_other = strlen((const char*) other);
    usize length = self->length < length_other ? self->length : length_other;
    return memcmp(self->buffer, other, length) == 0;
}

i32 string_compare(const String** self, const String** other) {
    usize length = (*self)->length < (*other)->length ? (*self)->length : (*other)->length;
    return memcmp((*self)->buffer, (*other)->buffer, length);
}

i32 str_compare(const Str** self, const Str** other) {
    usize length = (*self)->length < (*other)->length ? (*self)->length : (*other)->length;
    return memcmp((*self)->buffer, (*other)->buffer, length);
}

// PRINTING

void str_debug(const Str* self) {
    printf("{\n  Buffer: ");
    fwrite(self->buffer, sizeof(u8), self->length, stdout);
    printf("\n  Length: %zu\n}\n", self->length);
}

void str_println(const Str* self) {
    fwrite(self->buffer, sizeof(u8), self->length, stdout);
    fwrite("\n", sizeof(u8), 1, stdout);
}

void str_print(const Str* self) {
    fwrite(self->buffer, sizeof(u8), self->length, stdout);
}
