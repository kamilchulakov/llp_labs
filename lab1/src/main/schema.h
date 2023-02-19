#ifndef APP_SCHEMA_H
#define APP_SCHEMA_H

#include "bson.h"

typedef struct {
    uint32_t field_count;
    field* fields;
} schema;

WRITE_STATUS write_schema(FILE* fp, schema* sch);
READ_STATUS read_schema(FILE* fp, schema* sch);
bool schema_equals(schema* first, schema* second);
schema* new_schema(uint32_t field_count);

#endif //APP_SCHEMA_H
