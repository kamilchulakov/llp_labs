#ifndef ENORMEDB_QUERY_H
#define ENORMEDB_QUERY_H

#include "bson.h"
#include "schema.h"

typedef enum {ALL, ID, NAME} filter_type;

typedef union {
    uint32_t id;
    string str;
} filter_data;

typedef struct {
    filter_type type;
    filter_data* data;
} filter;

typedef enum { CREATE, DELETE, SCAN} schema_query_type;

typedef struct {
    schema schema;
} create_schema_query_data;

typedef struct {
    string schema_name;
} delete_schema_query_data;

typedef struct {
    filter schema_name_filter; // ALL or NAME
} scan_schema_query_data;

typedef union {
    create_schema_query_data create_query_data;
    delete_schema_query_data delete_query_data;
    scan_schema_query_data scan_query_data;
} schema_query_data;

typedef struct {
    schema_query_type query_type;
    schema_query_data query_data;
} schema_query;

#endif //ENORMEDB_QUERY_H
