#ifndef ENORMEDB_TYPES_H
#define ENORMEDB_TYPES_H

#include <inttypes.h>
#include "io.h"

typedef enum { false = 0, true = 1} bool;

READ_STATUS read_bool(FILE* fp, bool* bl);
WRITE_STATUS write_bool(FILE* fp, bool* bl);

typedef struct {
    size_t len;
    char* ch;
} string;

typedef struct string_part string_part;
struct string_part {
    string* part;
    uint32_t pageId;
    size_t len;
    string_part* nxt;
    uint32_t nxtPageId;
};

READ_STATUS read_string(FILE* fp, string* str);
WRITE_STATUS write_string(FILE* fp, string* str);
string* string_of(char* ch);
string* string_of_len(size_t len);
size_t string_size(string* str);
bool string_equals(string* first, string* second);

string_part* split_string(string* str, size_t start);
READ_STATUS read_string_split(FILE* fp, string_part *part);
READ_STATUS read_string_header_in_document(FILE* fp, string_part *part);
WRITE_STATUS write_string_split(FILE* fp, string_part *part);
WRITE_STATUS write_string_header_in_document(FILE* fp, string_part *part);

READ_STATUS read_uint(FILE* fp, uint32_t* val);
WRITE_STATUS write_uint(FILE* fp, uint32_t* val);
READ_STATUS read_double(FILE* fp, double* val);
WRITE_STATUS write_double(FILE* fp, double* val);
READ_STATUS read_int32(FILE* fp, int32_t* val);
WRITE_STATUS write_int32(FILE* fp, int32_t* val);

#endif //ENORMEDB_TYPES_H
