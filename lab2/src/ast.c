#include <stdio.h>
#include "ast.h"

schema_field* create_schema_field(char* name, char* type) {
    schema_field* fld = malloc(sizeof(schema_field));
    if (fld == NULL)
        return NULL;

    fld->name = realloc(name, strlen(name)+1);
    fld->type = realloc(name, strlen(type)+1);

    return fld;
}

db_query* create_create_collection_query(char* name, schema_field* schema) {
    create_collection_query* query = malloc(sizeof(create_collection_query));
    if (query == NULL)
        return NULL;

    query->name = realloc(name, strlen(name)+1);
    query->schema = schema;

    db_query* db_que = malloc(sizeof(db_query));
    if (db_que == NULL)
        return NULL;

    db_que->query.create_collection = query;
    db_que->type = CREATE_COLLECTION_QUERY;

    return db_que;
}

void print_db_query(db_query* db_que) {
    switch (db_que->type) {
        case CREATE_COLLECTION_QUERY:
            printf("CREATE_COLLECTION");
            break;
    }
}