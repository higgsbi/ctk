#include "path.h"
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../types/types.h"

#ifdef __APPLE__
#define HOME_STRING "/Users/"
#else
#define HOME_STRING "/home/"
#endif

// Ensures directories have trailing slash and are null terminated
// Length will not include null pointer
void path_adjust_to_native(Path* self) {
    ASSERT_NONNULL(self);

    if (self->buffer[self->length - 1] != '\0') {
        string_push_char(self, '\0');
    }

    if (self->buffer[self->length - 2] != '/' && path_is_directory(self)) {
        self->buffer[self->length - 1] = '/';
        string_push_char(self, '\0');
    }

    self->length--;
}

Path* path_from_string(String* string) {
    ASSERT_NONNULL(string);

    if (string->length == 0) {
        free(string->buffer);
        char* raw = getcwd(null, 0);
        Path* self = string;
        self->length = strlen(raw);
        self->capacity = self->length;
        self->buffer = raw;

        if (self->buffer[self->length - 1] != '/' && path_is_directory(self)) {
            self->buffer[self->length] = '/';
            self->length++;
            self->capacity++;
            self->buffer = reallocate(self->buffer, sizeof(u8) * (self->length + 1));
            self->buffer[self->length] = '\0';
        }

        return self;
    }

    path_adjust_to_native(string);
    return (Path*) string;
}

Path* path_from_str(const Str* str) {
    ASSERT_NONNULL(str);
    if (str->length == 0) {
        return path_current();
    }

    return path_from_string(string_from_str(str));
}

Path* path_root() {
    return (Path*) string_from_str(&str("/"));
}

Path* path_user() {
    char* user = getenv("USER");
    ASSERT_NONNULL(user);

    Path* self = (Path*) string_from_str(&str(HOME_STRING));
    string_push_cstr(self, user);
    path_adjust_to_native(self);

    return self;
}

Path* path_current() {
    char* raw = getcwd(null, 0);
    Path* self = allocate(sizeof(Path), 1);
    self->length = strlen(raw);
    self->capacity = self->length;
    self->buffer = raw;

    if (self->buffer[self->length - 1] != '/' && path_is_directory(self)) {
        self->buffer[self->length] = '/';
        self->length++;
        self->capacity++;
        self->buffer = reallocate(self->buffer, sizeof(u8) * (self->length + 1));
        self->buffer[self->length] = '\0';
    }

    return self;
}

Path* path_clone(Path* self) {
    ASSERT_NONNULL(self);
    return string_clone(self);
}

void path_free(Path* self) {
    ASSERT_NONNULL(self);
    string_free(self);
}

void path_resolve(Path* self, const Str* added) {
    ASSERT_NONNULL(self);
    ASSERT_NONNULL(added);
    string_push_str(self, added);
    path_adjust_to_native(self);
}

void path_normalize(Path* self) {
    ASSERT_NONNULL(self);
    Path* normalized = null;
    if (self->length == 0 || (self->length == 1 && self->buffer[0] != '~')) {
        return;
    }

    for (usize i = 0; i < self->length - 1; i++) {
        char current = self->buffer[i];
        char next = self->buffer[i + 1];

        if (i == 0) {
            if (current == '.' && next == '.') {
                normalized = path_current();
                path_to_parent(normalized);
                i += 2;
            } else if (current == '~') {
                normalized = path_user();
                i++;
            } else {
                char current_arr[] = {self->buffer[i]};
                normalized = path_from_str(&(Str) {current_arr, 1});
            }
        } else {
            if (current == '.' && next == '.') {
                path_to_parent(normalized);
                i += 2;
            } else {
                string_push_char(normalized, self->buffer[i]);
            }
        }
    }

    string_push_char(normalized, self->buffer[self->length - 1]);
    string_push_char(normalized, '\0');

    free(self->buffer);
    self->buffer = normalized->buffer;
    self->length = normalized->length;
    self->capacity = normalized->capacity;
    free(normalized);
}

Str path_extension_splice(Path* self) {
    ASSERT_NONNULL(self);
    OptionIndex dot_index = str_index_last_char(string_splice(self), '.');

    if (option_index_is_empty(dot_index)) {
        return str("");
    }

    return str_subsplice(string_splice(self), dot_index.value + 1, self->length - dot_index.value - 1);
}

void path_to_parent(Path* self) {
    ASSERT_NONNULL(self);

    if (self->length == 1 && self->buffer[0] == '/') {
        return;
    }

    if (self->length == 1 && self->buffer[0] == '~') {
        self->buffer = HOME_STRING;
        self->length = sizeof(HOME_STRING) - 1;
        return;
    }

    OptionIndex parent_slash = string_index_last_n_char(self, '/', 2);

    if (option_index_is_empty(parent_slash) || parent_slash.value == 0) {
        self->buffer = "/";
        self->length = 1;
    }

    self->buffer[parent_slash.value + 1] = '\0';
    self->length = parent_slash.value + 1;
}

bool path_exists(Path* self) {
    ASSERT_NONNULL(self);
    struct stat path_stat;
    return (lstat(self->buffer, &path_stat) == 0);
}

bool path_is_directory(Path* self) {
    ASSERT_NONNULL(self);
    struct stat path_stat;

    if (lstat(self->buffer, &path_stat) != 0) {
        return false;
    }

    return (path_stat.st_mode & S_IFDIR);
}
