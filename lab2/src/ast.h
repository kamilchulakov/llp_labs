#ifndef LAB2_AST_H
#define LAB2_AST_H

#include <stdlib.h>
#include <string.h>

typedef struct schema_field schema_field;

struct schema_field {
    char* name;
    char* type;
    schema_field* nxt;
};

typedef struct {
    char* name;
    schema_field* schema;
} create_collection_query;

typedef struct {
    char* value;
} str_query_criteria;

typedef struct {
    str_query_criteria* criteria;
} get_collection_query;

typedef enum { CREATE_COLLECTION_QUERY, GET_COLLECTION_QUERY } query_type;

typedef struct {
    union {
        create_collection_query* create_collection;
        get_collection_query* get_collection;
    } query;
    query_type type;
} db_query;

str_query_criteria* create_str_query_criteria(char* value);
schema_field* create_schema_field(char* name, char* type);
db_query* create_create_collection_query(char* name, schema_field* schema);
db_query* create_get_collection_query(str_query_criteria* criteria);
void print_db_query(db_query* db_que);


#endif //LAB2_AST_H
