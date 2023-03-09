#ifndef ENORMEDB_QUERY_H
#define ENORMEDB_QUERY_H

#include "bson.h"
#include "collection.h"
#include "element.h"

typedef struct {
    collection* col;
} create_schema_query;

typedef struct {
    string* collection;
} delete_schema_query;

typedef struct {
    string* collection;
} get_schema_query;

typedef struct {
    uint32_t parent_id;

} parent_ref;

typedef struct {
    parent_ref* parent; // nullable
    string* collection;
    document* doc;
} insert_query;

typedef struct {
    string* collection; // nullable
    complex_filter* filters; // only top level checks
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
    DOCUMENT_LIST_RESULT_TYPE,
    COLLECTION_RESULT_TYPE
} query_result_data_type;

typedef struct document_list document_list;
struct document_list{
    document* curr;
    document_list* nxt;
};

typedef struct {
    query_result_data_type type;
    uint32_t pageId;
    union {
        schema* schema1;
        schema* schemas;
        element* element1;
        document_list* documents;
        collection* col;
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
