#ifndef ENORMEDB_BSON_H
#define ENORMEDB_BSON_H

#include <string.h>
#define MAX_ELEMENT_NAME 64

typedef struct element element;
typedef enum ELEMENT_TYPE ELEMENT_TYPE;
typedef struct field field;

enum ELEMENT_TYPE {EMPTY, DOUBLE, STRING, EMBEDDED_DOC, ARRAY, BINARY, OBJECT_ID, BOOLEAN, UTC_DATETIME,
        NULL_TYPE, REGEX, INT32, TIMESTAMP, INT64, DECIMAL128, MIN_KEY, MAX_KEY};

struct field {
    ELEMENT_TYPE e_type;
    char e_name[MAX_ELEMENT_NAME];
};

struct element {
    field e_field;
    void* e_data;
};

#endif