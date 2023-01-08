#include <stdio.h>
#include "ast.h"

str_query_criteria* create_str_query_criteria(char* value) {
    str_query_criteria* criteria = malloc(sizeof(str_query_criteria));
    if (criteria == NULL)
        return NULL;

    criteria->value = malloc(strlen(value)+1);
    strcpy(criteria->value, value);
    return criteria;
}

schema_field* create_schema_field(char* name, char* type) {
    schema_field* fld = malloc(sizeof(schema_field));
    if (fld == NULL)
        return NULL;

    fld->name = malloc(strlen(name)+1);
    fld->type = malloc(strlen(type)+1);
    strcpy(fld->name, name);
    strcpy(fld->type, type);

    return fld;
}

db_query* create_create_collection_query(char* name, schema_field* schema) {
    create_collection_query* query = malloc(sizeof(create_collection_query));
    if (query == NULL)
        return NULL;

    query->name = malloc(strlen(name)+1);
    strcpy(query->name, name);
    query->schema = schema;

    db_query* db_que = malloc(sizeof(db_query));
    if (db_que == NULL)
        return NULL;

    db_que->query.create_collection = query;
    db_que->type = CREATE_COLLECTION_QUERY;

    return db_que;
}

db_query* create_get_collection_query(str_query_criteria* criteria) {
    get_collection_query* query = malloc(sizeof(get_collection_query));
    if (query == NULL)
        return NULL;

    query->criteria = criteria;

    db_query* db_que = malloc(sizeof(db_query));
    if (db_que == NULL)
        return NULL;

    db_que->query.get_collection = query;
    db_que->type = GET_COLLECTION_QUERY;

    return db_que;
}

void print_tabs(int tabs) {
    for (int i = 0; i < tabs; i++) {
        printf("\t");
    }
}

void print_schema_field(schema_field* fld, int tabs) {
    print_tabs(tabs);
    printf("schema_field:\n");
    print_tabs(tabs+1);
    printf("field_name: %s\n", fld->name);
    print_tabs(tabs+1);
    printf("field_type: %s\n", fld->type);
}

void print_schema(schema_field* schema, int tabs) {
    schema_field* curr = schema;
    print_tabs(tabs);
    printf("schema:\n");
    int curr_tabs = tabs+1;
    while (curr->nxt) {
        print_schema_field(curr, curr_tabs);
        curr = curr->nxt;
    }
    print_schema_field(curr, curr_tabs);
}

void print_create_collection(db_query* db_que) {
    int tabs = 1;
    printf("createCollection:\n");
    print_tabs(tabs);
    printf("collection: %s\n", db_que->query.create_collection->name);
    print_schema(db_que->query.create_collection->schema, tabs);
}

void print_get_collection(db_query* db_que) {
    printf("getCollection:\n");
    print_tabs(1);
    printf("collection: %s\n", db_que->query.get_collection->criteria->value);
}

void print_db_query(db_query* db_que) {
    printf("\nOUTPUT:\n");
    switch (db_que->type) {
        case CREATE_COLLECTION_QUERY:
            print_create_collection(db_que);
            break;
        case GET_COLLECTION_QUERY:
            print_get_collection(db_que);
            break;
    }
}