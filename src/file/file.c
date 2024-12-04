#include "file.h"
#include <stdarg.h>
#include <stdio.h>
#include "../io.h"
#include "path.h"

File file_from_path(Path* path) {
    ASSERT_NONNULL(path);
    return (File) {.path = path, .buffer = null};
}

void file_create(File* self, bool overwrite) {
    ASSERT_NONNULL(self);
    if (!file_exists(self) || overwrite) {
        file_open(self, FILE_OVERWRITE);
        file_close(self);
    }
}

void file_delete(File* self) {
    ASSERT_NONNULL(self);
    remove(self->path->buffer);
}

bool file_exists(File* self) {
    return path_exists(self->path);
}

void file_open(File* self, FileMode* mode) {
    ASSERT_NONNULL(self);
    ASSERT_NONNULL(mode);
    self->buffer = fopen(self->path->buffer, mode->buffer);
    ASSERT_NONNULL(self->buffer);
}

void file_close(File* self) {
    ASSERT_NONNULL(self);
    ASSERT_NONNULL(self->buffer);
    fclose(self->buffer);
}

void file_reset(File* self) {
    ASSERT_NONNULL(self);
    ASSERT_NONNULL(self->buffer);
    fseek(self->buffer, 0, SEEK_SET);
}

void file_free(File* self) {
    if (self->buffer != null) {
        file_close(self);
    }
    path_free(self->path);
}

void file_write_str(File* self, Str* str) {
    ASSERT_NONNULL(self);
    ASSERT_NONNULL(self->buffer);
    ASSERT_NONNULL(str);

    fwrite(str->buffer, sizeof(c8), str->length, self->buffer);
}

void file_write_line(File* self, Str* str) {
    ASSERT_NONNULL(self);
    ASSERT_NONNULL(self->buffer);
    ASSERT_NONNULL(str);

    fwrite(str->buffer, sizeof(c8), str->length, self->buffer);
    fwrite("\n", sizeof(c8), 1, self->buffer);
}

void file_write(File* self, const Str* format, ...) {
    va_list args;
    va_start(args, format);
    fprint_args(self->buffer, format, args);
    va_end(args);
}

VecString* file_read_lines(File* self) {
    String* line = string_empty();
    VecString* vec = vec_string_new(DEFAULT_LINES_PER_FILE);

    while (file_read_line(self, line)) {
        vec_string_push_back(vec, string_clone(line));
    }

    string_free(line);
    return vec;
}

bool file_read_line(File* self, String* buffer) {
    ASSERT_NONNULL(self);
    ASSERT_NONNULL(self->buffer);
    ASSERT_NONNULL(buffer);

    buffer->length = 0;
    c8 current;
    bool eof = false;

    while ((current = fgetc(self->buffer))) {
        if (current == '\0') {
            break;
        }

        if (current == -1) {
            eof = true;
            break;
        }

        if (current == '\n') {
            break;
        }

        string_push_char(buffer, current);
    }

    return !eof || buffer->length > 0;
}
