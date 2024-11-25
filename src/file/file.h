#ifndef CTK_FILE_H
#define CTK_FILE_H

#include <stdio.h>
#include "path.h"

#define FILE_READ &str("r")
#define FILE_OVERWRITE &str("w+")
#define FILE_APPEND &str("a+")

typedef Str FileMode;

typedef struct {
    Path* path;
    FILE* buffer;
} File;

#define DEFAULT_LINES_PER_FILE 10

File file_from_path(Path* path);
void file_create(File* self, bool overwrite);
void file_delete(File* self);
bool file_exists(File* self);
void file_open(File* self, FileMode* mode);
void file_close(File* self);
void file_reset(File* self);

void file_write(File* self, Str* str);
void file_writef(File* self, const char* format, ...);
void file_write_line(File* self, Str* str);

bool file_read_line(File* self, String* buffer);
VecString* file_read_lines(File* self);

/**
 * Note: frees the path and closes file
 * Does not free the given file pointer
 * Assumes this file is not heap allocated
 */
void file_free(File* self);

#endif
