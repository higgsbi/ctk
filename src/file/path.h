#ifndef CTK_PATH_H
#define CTK_PATH_H

#include "../types/string.h"

typedef String Path;

Path* path_from_string(String* string);
Path* path_from_str(const Str* str);
Path* path_current();
Path* path_root();
Path* path_user();
Path* path_clone(Path* self);
void path_free(Path* self);

void path_to_parent(Path* self);
void path_resolve(Path* self, const Str* added);
void path_normalize(Path* self);

bool path_exists(Path* self);
bool path_is_directory(Path* self);
Str path_extension_splice(Path* self);

#endif
