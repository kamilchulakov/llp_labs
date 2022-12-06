#ifndef APP_SCHEMA_H
#define APP_SCHEMA_H

#include "bson.h"

typedef struct {
    size_t field_count;
    field* fields;
} schema_data;

typedef struct {
    string name;
    schema_data data;
} schema;

WRITE_STATUS write_schema(FILE* fp, schema* sch);

#endif //APP_SCHEMA_H
