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

db_query* create_drop_database_query() {
    db_query* db_que = malloc(sizeof(db_query));
    if (db_que == NULL)
        return NULL;

    db_que->type = DROP_DATABASE_QUERY;
    return db_que;
}


value* create_int32_value(int val) {
    value* res = malloc(sizeof(value));
    if (res == NULL)
        return NULL;
    res->type = INT32_VAL_TYPE;
    res->intval = val;
    return res;
}

value* create_str_value(char* val) {
    value* res = malloc(sizeof(value));
    if (res == NULL)
        return NULL;
    res->type = STR_VAL_TYPE;
    res->strval = malloc(strlen(val)+1);
    strcpy(res->strval, val);
    return res;
}

query_criteria* create_field_criteria(char* field, int cmp_op, value* val) {
    query_criteria* criteria = malloc(sizeof(query_criteria));
    if (criteria == NULL) {
        return NULL;
    }
    criteria->fld = malloc(sizeof(field_query_criteria));
    if (criteria->fld == NULL)
        return NULL;

    criteria->type = FIELD_CRITERIA;
    criteria->fld->field = malloc(strlen(field)+1);
    strcpy(criteria->fld->field, field);
    criteria->fld->val = val;
    criteria->fld->type = cmp_op;

    return criteria;
}

query_criteria* create_criteria_operator(int criteria_op, query_criteria* other) {
    query_criteria* criteria = malloc(sizeof(query_criteria));
    if (criteria == NULL) {
        return NULL;
    }
    criteria->op = malloc(sizeof(operator_criteria));
    if (criteria->op == NULL)
        return NULL;

    criteria->type = OP_CRITERIA;
    criteria->op->type = criteria_op;
    criteria->op->criterias = other;
    return criteria;
}

collection_query* create_count_query(query_criteria* criteria) {
    count_query* query = malloc(sizeof(count_query));
    if (query == NULL)
        return NULL;

    query->criteria = criteria;

    collection_query* que = malloc(sizeof(collection_query));
    if (que == NULL)
        return NULL;

    que->query.count = query;
    que->type = COUNT_QUERY;

    return que;
}

collection_query* create_find_query(query_criteria* criteria, int limit) {
    find_query* query = malloc(sizeof(find_query));
    if (query == NULL)
        return NULL;

    query->criteria = criteria;
    query->limit = limit;

    collection_query* que = malloc(sizeof(collection_query));
    if (que == NULL)
        return NULL;

    que->query.find = query;
    que->type = FIND_QUERY;

    return que;
}

// ________________ prints ____________

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
        case DROP_DATABASE_QUERY:
            printf("dropDatabase\n");
            break;
    }
}
void print_value(value* val) {
    switch (val->type) {
        case INT32_VAL_TYPE:
            printf("value: %d\n", val->intval);
            break;
        case STR_VAL_TYPE:
            printf("value: %s\n", val->strval);
            break;
    }
}

void print_operator(int tabs, int type) {
    print_tabs(tabs);
    switch (type) {
        case CMP_EQ:
            printf("op: equal\n");
            break;
        case CMP_LT:
            printf("op: less\n");
            break;
        case CMP_LTE:
            printf("op: less or equal\n");
            break;
        case CMP_NEQ:
            printf("op: not equal\n");
            break;
        case CMP_REGEX:
            printf("op: regex\n");
            break;
        case CMP_GT:
            printf("op: greater\n");
            break;
        case CMP_GTE:
            printf("op: greater or equal\n");
            break;
    }
}

void print_field_criteria(field_query_criteria* fld, int tabs) {
    print_tabs(tabs);
    printf("field_criteria:\n");
    print_tabs(tabs+1);
    printf("field: %s\n", fld->field);
    print_operator(tabs+1, fld->type);
    print_tabs(tabs+1);
    print_value(fld->val);
}

void print_criteria_operator(int tabs, int type) {
    print_tabs(tabs);
    switch (type) {
        case 0:
            printf("operator: or\n");
            break;
        case 1:
            printf("operator: and\n");
            break;
    }
}
void print_query_criteria(query_criteria* criteria, int tabs);
void print_operator_criteria(operator_criteria* op, int tabs) {
    print_tabs(tabs);
    printf("operator_criteria:\n");
    print_criteria_operator(tabs+1, op->type);
    print_query_criteria(op->criterias, tabs+1);
}

void print_query_criteria(query_criteria* criteria, int tabs) {
    print_tabs(tabs);
    printf("criteria:\n");
    int curr_tabs = tabs+1;
    query_criteria* curr = criteria;
    while (curr) {
        switch (curr->type) {
            case OP_CRITERIA:
                print_operator_criteria(curr->op, curr_tabs);
                break;
            case FIELD_CRITERIA:
                print_field_criteria(curr->fld, curr_tabs);
                break;
        }
        curr = curr->nxt;
    }
}

void print_count(collection_query* col_query) {
    int tabs = 1;
    printf("count:\n");
    print_tabs(tabs);
    printf("collection: %s\n", col_query->collection);
    print_query_criteria(col_query->query.count->criteria, tabs);
}
void print_find(collection_query* col_query) {
    int tabs = 1;
    printf("find:\n");
    if (col_query->query.find->limit != -1) {
        print_tabs(tabs);
        printf("limit: %d\n", col_query->query.find->limit);
    }
    print_tabs(tabs);
    printf("collection: %s\n", col_query->collection);
    print_query_criteria(col_query->query.count->criteria, tabs);
}

void print_col_query(collection_query* col_query) {
    printf("\nOUTPUT:\n");
    switch (col_query->type) {
        case COUNT_QUERY:
            print_count(col_query);
            break;
        case FIND_QUERY:
            print_find(col_query);
            break;
    }
}