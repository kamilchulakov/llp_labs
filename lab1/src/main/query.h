#ifndef ENORMEDB_QUERY_H
#define ENORMEDB_QUERY_H

#include "bson.h"
#include "schema.h"
#include "element.h"

typedef enum {ALL, ID, NAME} filter_type;

typedef struct {
    filter_type type;
    union {
        uint32_t id;
        string str;
    };
} filter;

typedef struct {
    schema* schema;
    string* collection;
} create_schema_query;

typedef struct {
    string* collection;
} delete_schema_query;

typedef struct {
    string* collection;
} get_schema_query;

typedef struct {
    uint32_t parent_id; // nullable
    string* collection;
    element* elements; // can be doc, but doc is harder to create
} insert_query;

typedef struct {
    string* collection;
    element_filter* filters; // can process AND/OR in different place
} find_query;

typedef struct {
    string* collection;
    element_filter* filters;
    element* elements;
} update_query;

//_______________result________________
typedef enum {
    SCHEMA_RESULT_TYPE,
    SCHEMA_ARRAY_RESULT_TYPE,
    DOCUMENT_RESULT_TYPE,
    DOCUMENT_ARRAY_RESULT_TYPE
} query_result_data_type;

typedef struct {
    query_result_data_type type;
    union {
        schema* schema1;
        schema* schemas;
        element* element1;
        element* elements;
    };
} query_result_data;

typedef enum {BOOL_RESULT_TYPE, ERR_RESULT_TYPE, DATA_RESULT_TYPE} query_result_type;

typedef struct {
    query_result_type type;
    union {
        bool ok;
        string* err;
        query_result_data* data;
    };
} query_result;

#endif //ENORMEDB_QUERY_H
