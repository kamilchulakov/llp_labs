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

typedef enum { false = 0, true } bool;
typedef enum { INT32_VAL_TYPE, STR_VAL_TYPE, BOOL_VAL_TYPE, DOUBLE_VAL_TYPE } value_type;
typedef struct {
    union {
        int intval;
        char* strval;
        bool boolval;
        double doubleval;
    };
    value_type type;
} value;

typedef struct field_value field_value;
struct field_value {
    char* field;
    value* val;
    field_value* nxt;
};

typedef struct document document;
struct document {
    field_value* elements;
    document* nxt;
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

typedef enum { CREATE_COLLECTION_QUERY, GET_COLLECTION_QUERY, DROP_DATABASE_QUERY } db_query_type;

typedef enum {
    CMP_EQ = 0,
    CMP_NEQ = 1,
    CMP_GT = 2,
    CMP_GTE = 3,
    CMP_LT = 4,
    CMP_LTE = 5,
    CMP_REGEX = 6
} op_type;

typedef struct field_query_criteria field_query_criteria;
struct field_query_criteria {
    char* field;
    op_type type;
    value* val;
};

typedef struct {
    union {
        create_collection_query* create_collection;
        get_collection_query* get_collection;
    } query;
    db_query_type type;
} db_query;


typedef struct query_criteria query_criteria;

typedef enum {
    OR_CRITERIA = 0,
    AND_CRITERIA = 1
} criteria_op_type;
typedef struct {
    query_criteria* criterias;
    criteria_op_type type;
} operator_criteria;

typedef enum { FIELD_CRITERIA, OP_CRITERIA } query_criteria_type;
struct query_criteria {
    union {
        field_query_criteria* fld;
        operator_criteria* op;
    };
    query_criteria_type type;
    query_criteria* nxt;
};

typedef struct {
    query_criteria* criteria;
} count_query;

typedef struct {
    query_criteria* criteria;
    int limit;
} find_query;

typedef struct {
    query_criteria* criteria;
    int limit;
} remove_query;

typedef struct {
    document* docs;
} insert_query;

typedef struct {
    query_criteria* criteria;
    document* doc;
} update_query;

typedef enum { COUNT_QUERY, FIND_QUERY, REMOVE_QUERY, INSERT_QUERY, UPDATE_QUERY } collection_query_type;

typedef struct {
    union {
        count_query* count;
        find_query* find;
        remove_query* remove;
        insert_query* insert;
        update_query* update;
    } query;
    collection_query_type type;
    char* collection;
} collection_query;


str_query_criteria* create_str_query_criteria(char* value);
schema_field* create_schema_field(char* name, char* type);
db_query* create_create_collection_query(char* name, schema_field* schema);
db_query* create_get_collection_query(str_query_criteria* criteria);
db_query* create_drop_database_query();

value* create_int32_value(int val);
value* create_str_value(char* val);
value* create_bool_value(bool val);
value* create_double_value(double val);

document* create_document(field_value* elements);
field_value* create_field_value(char* field, value* val);

query_criteria* create_field_criteria(char* field, int cmp_op, value* val);
query_criteria* create_criteria_operator(int criteria_op, query_criteria* other);

collection_query* create_count_query(query_criteria* criteria);
collection_query* create_find_query(query_criteria* criteria, int limit);
collection_query* create_remove_query(query_criteria* criteria, int limit);
collection_query* create_update_query(query_criteria* criteria, document* doc);
collection_query* create_insert_query(document* docs);

void print_db_query(db_query* db_que);
void print_col_query(collection_query* col_query);

void free_db_query(db_query* db_que);
void free_col_query(collection_query* col_query);


#endif //LAB2_AST_H
