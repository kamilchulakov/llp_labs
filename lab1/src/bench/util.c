#include "util.h"
#include "../test/test.h"

void insert_schema(db_handler* db) {
    create_schema_query query = {new_collection(string_of("mem"), new_schema(4))};
    query.col->sch->fields[0].e_type = INT32;
    query.col->sch->fields[0].e_name = string_of("int_field");
    query.col->sch->fields[1].e_type = BOOLEAN;
    query.col->sch->fields[1].e_name = string_of("bool_field");
    query.col->sch->fields[2].e_type = DOUBLE;
    query.col->sch->fields[2].e_name = string_of("double_field");
    query.col->sch->fields[3].e_type = STRING;
    query.col->sch->fields[3].e_name = string_of("string_field");
    query_result res = create_schema(db, &query);
    free_result(res);
    free_collection(query.col);
}

void insert_documents(db_handler *db, int amount, clock_t *time) {
    insert_query query = {NULL, string_of("mem"), NULL};
    for (int i = 0; i < amount; ++i) {
        query.doc = create_document(4);
        query.doc->data.elements[0] = *create_element(INT32, "int_field");
        query.doc->data.elements[0].int_data = i;
        query.doc->data.elements[1] = *create_element(BOOLEAN, "bool_field");
        query.doc->data.elements[1].bool_data = i % 2;
        query.doc->data.elements[2] = *create_element(DOUBLE, "double_field");
        query.doc->data.elements[2].double_data = ((double) i) / 3;
        query.doc->data.elements[3] = *create_element(STRING, "string_field");
        query.doc->data.elements[3].string_data = bigString();
        query_result res = collection_insert(db, &query);
        if (time != NULL) *time = clock();
        free_result(res);
        //  FREE free_document_with_strings(query.doc);
    }
    free_string(query.collection);
}

void collection_find_all(db_handler *db, clock_t* time) {
    find_query findQuery = {string_of("mem"), NULL};
    query_result res = collection_find(db, &findQuery);
    if (time != NULL) *time = clock();
//    free_result(res);
    free_string(findQuery.collection);
}

void collection_find_true(db_handler *db, clock_t *time) {
    find_query findQuery = {string_of("mem"), malloc(sizeof(complex_filter))};
    findQuery.filters->type = ELEMENT_FILTER;
    findQuery.filters->el_filter = create_element_filter(CMP_EQ,create_element(BOOLEAN, "bool_field"));
    findQuery.filters->el_filter->el->bool_data = true;
    query_result res = collection_find(db, &findQuery);
    *time = clock();
    free_result(res);
    free_string(findQuery.collection);
    free_field(findQuery.filters->el_filter->el->e_field);
    free(findQuery.filters->el_filter->el);
    free(findQuery.filters->el_filter);
    free(findQuery.filters);
}

void clear_all_documents(db_handler* db) {
    find_query findQuery = {string_of("mem"), NULL};
    collection_remove(db, &findQuery);
    free_string(findQuery.collection);
}