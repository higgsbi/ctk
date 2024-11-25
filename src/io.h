#ifndef CTK_INCLUDE_H
#define CTK_INCLUDE_H

/**
 * CToolkit standard library
 *
 * Included:
 *
 * <stdint.h>
 * <stdbool.h>
 * <stddef.h>
 * <stdlib.h>
 * "types/string.h"
 * "types/type.h"
 * "types/option.h"
 */

#include <stdio.h>
#include "assert.h"
#include "types/option.h"
#include "types/string.h"

void ctk_printf(const char* format, ...);

/** Types:
 *
 * %S -> String
 * %p -> path
 * %s -> str
 * %r -> raw cstring
 * %i -> int
 * %f -> float
 * %b -> bool
 * %a -> address
 */
void ctk_fprintf(FILE* file, const char* format, ...);
void ctk_fprint_args(FILE* file, const char* format, va_list args);

#endif
