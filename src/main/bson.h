#ifndef ENORMEDB_BSON_H
#define ENORMEDB_BSON_H

#include <string.h>

typedef enum {EMPTY, DOUBLE, STRING, EMBEDDED_DOC, ARRAY, BINARY, OBJECT_ID, BOOLEAN, UTC_DATETIME,
        NULL_TYPE, REGEX, INT32, TIMESTAMP, INT64, DECIMAL128, MIN_KEY, MAX_KEY} ELEMENT_TYPE;

typedef struct {
    ELEMENT_TYPE e_type;
    char* e_name;
} field;

#endif
