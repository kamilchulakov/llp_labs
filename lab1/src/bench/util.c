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
    create_schema(db, &query);
}

void insert_documents(db_handler* db, int amount) {
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
        collection_insert(db, &query);
        free_document(query.doc);
    }
}

void free_result(query_result res) {
    if (res.type == DATA_RESULT_TYPE) {
        if (res.data->type == DOCUMENT_LIST_RESULT_TYPE) {
            document_list* curr = res.data->documents;
            while (curr && curr->currDoc) {
                document_list* prev = curr;
                curr = curr->nxt;

                free(prev);
            }
        }
        free(res.data);
    }
}

void collection_find_all(db_handler* db) {
    find_query findQuery = {string_of("mem"), NULL};
    free_result(collection_find(db, &findQuery));
}

void collection_find_true(db_handler* db) {
    find_query findQuery = {string_of("mem"), malloc(sizeof(complex_filter))};
    findQuery.filters->type = ELEMENT_FILTER;
    findQuery.filters->el_filter = create_element_filter(CMP_EQ,create_element(BOOLEAN, "bool_field"));
    findQuery.filters->el_filter->el->bool_data = true;
    free_result(collection_find(db, &findQuery));
}

void clear_all_documents(db_handler* db) {
    find_query findQuery = {string_of("mem"), NULL};
    collection_remove(db, &findQuery);
}