#ifndef ENORMEDB_BSON_H
#define ENORMEDB_BSON_H

#include "types.h"

typedef enum {EMPTY, DOUBLE, STRING, EMBEDDED_DOC, ARRAY, BINARY, OBJECT_ID, BOOLEAN, UTC_DATETIME,
        NULL_TYPE, REGEX, INT32, TIMESTAMP, INT64, DECIMAL128, MIN_KEY, MAX_KEY} ELEMENT_TYPE;

typedef struct {
    ELEMENT_TYPE e_type;
    string e_name;
} field;

//size_t size_of(field fld) {
//    return string_size(&fld.e_name) + sizeof(field);
//}

READ_STATUS read_field(FILE* fp, field e_field);
WRITE_STATUS write_field(FILE* fp, field e_field);

#endif
