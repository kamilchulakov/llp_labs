#ifndef ENORMEDB_BSON_H
#define ENORMEDB_BSON_H

#include "types.h"

typedef enum {DOUBLE, STRING, BOOLEAN, INT32} ELEMENT_TYPE;

typedef struct {
    ELEMENT_TYPE e_type;
    string* e_name;
} field;

READ_STATUS read_field(FILE* fp, field* e_field);
WRITE_STATUS write_field(FILE* fp, field* e_field);
field* empty_field();

#endif
