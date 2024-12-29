#ifndef CTK_STRING_H
#define CTK_STRING_H

#include "../collections/vector.h"
#include "option.h"

#define STRING_DEFAULT_LENGTH 8

// Non-null terminated, heap allocated
typedef struct {
    c8* buffer;
    usize length;
    usize capacity;
} String;

// Non-null terminated, heap or stack allocated, constant (used for keys, string slices),u
typedef struct {
    const c8* buffer;
    usize length;
} Str;

// Null terminated, heap allocated
typedef struct {
    c8* buffer;
    usize length;
    usize capacity;
} CString;

// Null terminated, heap or stack allocated, constant (used for keys, string slices)
typedef struct {
    const c8* buffer;
    usize length;
} CStr;

TYPE_OPTION(usize, Index, index, 0)

// CSTRING

#define cstr(literal) (CStr){.buffer = literal, .length = sizeof(literal) - 2}

CStr cstr_from_chars(const c8* chars);
CStr* cstr_owned(const c8* chars);
CString* cstring_from_str(const Str* str);
CString* cstring_from_string(String* self);
CString* cstring_from_format(const Str* format, ...);
CString* cstring_from_args(const Str* format, va_list args);
CStr* cstring_slice(const CString* self);
CString* cstring_clone(const CString* self);
CStr* cstr_clone_deep(const CStr* self);
void cstring_free(CString* self);
void cstr_free_deep(CStr* self);
void cstring_push_str(CString* self, const Str* str);
void cstring_push_cstr(CString* self, const CStr* str);
void cstring_push_char(CString* self, c8 character);

// STRING

// LIFECYCLE
#define str(literal) (Str){.buffer = literal, .length = sizeof(literal) - 1}

Str str_from_cstr(const CStr* cstr);
Str str_from_chars(const c8* chars);
Str* str_owned(const c8* chars);
Str* str_to_owned(const Str* self);
Str* string_slice(const String* self);
String* string_empty();
String* string_from_str(const Str* str);
String* string_from_format(const Str* format, ...);
String* string_from_args(const Str* format, va_list args);
String* string_with_capacity(usize capacity);
String* string_clone(const String* self);
void string_free(String* self);
Str* str_clone(const Str* self);
Str* str_clone_deep(const Str* self);
void str_free(Str* self);
void str_free_deep(Str* self);

// COMPARISON
bool string_equals_string(const String* self, const String* other);
bool string_equals_str(const String* self, const Str* other);
bool string_equals_chars(const String* self, const u8* other);
bool str_equals_str(const Str* self, const Str* other);
bool str_equals_chars(const Str* self, const u8* other);
i32 string_compare(const String** self, const String** other);
i32 str_compare(const Str** self, const Str** other);

// QUERY
bool str_contains_str(const Str* self, const Str* target);
bool str_contains_char(const Str* self, char target);
usize str_hash(const Str* self);

// SPLIT
TYPE_VECTOR(Str, StrSlice, str_slice, str_clone, str_free, str_compare)
TYPE_VECTOR(String, StringOwned, string_owned, string_clone, string_free, string_compare)
Str str_subslice(const Str* self, usize start, usize length);
VecStringOwned* str_split(const Str* self, char delimiter);
VecStrSlice* str_split_slice(const Str* self, char delimiter);

// INDEX
OptionIndex str_index_first_char(const Str* self, c8 character);
OptionIndex str_index_last_char(const Str* self, c8 character);
OptionIndex string_index_first_n_char(String* self, c8 character, usize n);
OptionIndex string_index_last_n_char(String* self, c8 character, usize n);

// MUTATION
void string_replace(String* self, Str* replaced, Str* replacer);
void string_push_str(String* self, const Str* str);
void string_push_cstr(String* self, const CStr* cstr);
void string_push_char(String* self, c8 character);
void string_trim(String* self);

void str_debug(const Str* self);
void str_println(const Str* self);
void str_print(const Str* self);

#endif
