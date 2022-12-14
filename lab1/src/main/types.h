#ifndef ENORMEDB_TYPES_H
#define ENORMEDB_TYPES_H

#include <inttypes.h>
#include "io.h"

typedef enum { false = 0, true } bool;

READ_STATUS read_bool(FILE* fp, bool* bl);
WRITE_STATUS write_bool(FILE* fp, bool* bl);

typedef struct {
    int len;
    char* ch;
} string;

READ_STATUS read_string(FILE* fp, string* str);
WRITE_STATUS write_string(FILE* fp, string* str);
size_t string_size(string* str);
bool string_equals(void* first, void* second);

#endif //ENORMEDB_TYPES_H
