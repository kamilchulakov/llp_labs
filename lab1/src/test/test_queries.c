#include "test_queries.h"
#include "../main/query_executor.h"
#include "../main/pager.h"

void test_create_schema(db_handler* db) {
    print_running_test("test_create_schema");

    create_schema_query query = {new_collection(string_of("ex"), new_schema(1))};
    query.col->sch->fields[0].e_name = string_of("my name");

    query_result result = create_schema(db, &query);
    assert_result_type(result, BOOL_RESULT_TYPE);
    assert_true(result.ok);

    get_schema_query query1 = {string_of("ex")};
    schema* written_schema = get_schema(db, &query1).data->schema1;
    assert(written_schema->field_count == 1);
    assert_field_equals(written_schema->fields, query.col->sch->fields);
}

void test_delete_schema(db_handler* db) {
    print_running_test("test_delete_schema");

    delete_schema_query query = {string_of("ex")};
    query_result result = delete_schema(db, &query);
    assert_result_type(result, BOOL_RESULT_TYPE);
    assert_true(result.ok);

    get_schema_query query1 = {string_of("ex")};
    result = get_schema(db, &query1);
    assert_false(result.ok);
}

void test_insert_document(db_handler* db) {
    print_running_test("test_insert_document");

    create_schema_query schemaQuery = {new_collection(string_of("ex"), new_schema(1))};
    schemaQuery.col->sch->fields->e_name = string_of("int");
    schemaQuery.col->sch->fields->e_type = INT32;
    create_schema(db, &schemaQuery);

    collection* col = get_collection(db, 1);
    assert(col->lastDocPageId == -1);
    assert(col->doc_page_id == -1);

    insert_query insertQuery = {NULL, string_of("ex"), create_document(1)};
    insertQuery.doc->data.elements->e_field = empty_field();
    insertQuery.doc->data.elements->e_field->e_name = string_of("not int");
    query_result res = collection_insert(db, &insertQuery);
    assert_false(res.ok);

    insertQuery.doc->data.elements->e_field->e_name = string_of("int");
    insertQuery.doc->data.elements->e_field->e_type = DOUBLE;
    res = collection_insert(db, &insertQuery);
    assert_false(res.ok);

    insertQuery.doc->data.elements->e_field->e_type = INT32;
    res = collection_insert(db, &insertQuery);
    assert_true(res.ok);

    page* pg = get_page(db, 2);
    assert(pg->type == PAGE_DOCUMENT);
    assert(pg->nextPageId == -1);
    assert(pg->prevPageId == -1);

    col = get_collection(db, 1);
    assert(col->lastDocPageId == 2);
    assert(col->doc_page_id == -1);
}

void test_insert_document_with_parent(db_handler* db) {
    print_running_test("test_insert_document_with_parent");

    parent_ref* parent = malloc(sizeof(parent_ref));
    parent->parent_id = 1;
    insert_query insertQuery = {parent, string_of("ex"), create_document(1)};
    insertQuery.doc->data.elements = create_element_int32("int", 32);
    query_result res = collection_insert(db, &insertQuery);
    assert_false(res.ok);

    parent->parent_id = 2;
    res = collection_insert(db, &insertQuery);
    assert_true(res.ok);

    document* parentDoc = get_document(db, 2);
    assert(parentDoc->childPage == 3);
    assert(db->pagerData->lastDocumentPage == 3);
    assert(get_collection(db, 1)->lastDocPageId == 3);

    parent->parent_id = 2;
    res = collection_insert(db, &insertQuery);
    assert_true(res.ok);
    assert(db->pagerData->lastDocumentPage == 4);
    assert(get_collection(db, 1)->lastDocPageId == 4);

    parentDoc = get_document(db, 2);
    document* newDoc = get_document(db, 4);
    assert(parentDoc->childPage == 4);
    assert(newDoc->prevCollectionDocument == 3);
    assert(get_collection(db, 1)->lastDocPageId == 4);

    parent->parent_id = 4;
    res = collection_insert(db, &insertQuery);
    assert_true(res.ok);

    parentDoc = get_document(db, 4);
    assert(parentDoc->childPage == 5);
    assert(parentDoc->prevBrotherPage == 3);
}

void assert_documents_count(query_result *result, uint32_t expected) {
    uint32_t count = 0;
    document_list* curr = (*result).data->documents;
    while (curr->currDoc) {
        count++;
        curr = curr->nxt;
    }
    assert(count == expected);
}

void test_find_all(db_handler* db) {
    print_running_test("test_find_all");

    query_result result = find_all(db);
    assert_result_type(result, DATA_RESULT_TYPE);
    assert(result.data->type == DOCUMENT_LIST_RESULT_TYPE);
    assert_documents_count(&result, 4);
}

void test_collection_find(db_handler* db) {
    print_running_test("test_collection_find");

    find_query query = {string_of("ex"), NULL};
    query_result result = collection_find(db, &query);
    assert_result_type(result, DATA_RESULT_TYPE);
    assert(result.data->type == DOCUMENT_LIST_RESULT_TYPE);
    assert_documents_count(&result, 4);

    query.filters = malloc(sizeof(complex_filter));
    query.filters->type = ELEMENT_FILTER;
    query.filters->el_filter = create_element_filter(CMP_LT, create_element_int32("int", 1));
    result = collection_find(db, &query);
    assert_result_type(result, DATA_RESULT_TYPE);
    assert(result.data->type == DOCUMENT_LIST_RESULT_TYPE);
    assert_documents_count(&result, 1);

    query.filters->el_filter = create_element_filter(CMP_EQ, create_element_int32("int", 32));
    result = collection_find(db, &query);
    assert_result_type(result, DATA_RESULT_TYPE);
    assert(result.data->type == DOCUMENT_LIST_RESULT_TYPE);
    assert_documents_count(&result, 3);

    insert_query insertQuery = {NULL, string_of("ex"), create_document(1)};
    insertQuery.doc->data.elements = create_element_int32("int", 32);
    assert_true(collection_insert(db, &insertQuery).ok);
    result = collection_find(db, &query);
    assert_documents_count(&result, 4);
}

void test_collection_remove(db_handler* db) {
    print_running_test("test_collection_remove");

    find_query query = {string_of("ex"), NULL};
    query.filters = malloc(sizeof(complex_filter));
    query.filters->type = ELEMENT_FILTER;
    query.filters->el_filter = create_element_filter(CMP_EQ, create_element_int32("int", 32));
    assert_true(collection_remove(db, &query).ok);

    query.filters = NULL;
    query_result result = collection_find(db, &query);
    assert_documents_count(&result, 1);
    assert(db->pagerData->page_id_seq == 7);
    assert(db->pagerData->first_free_document_page_id == 3);

    insert_query insertQuery = {NULL, string_of("ex"), create_document(1)};
    insertQuery.doc->data.elements = create_element_int32("int", 32);
    assert_true(collection_insert(db, &insertQuery).ok);
    assert_true(collection_insert(db, &insertQuery).ok);
    assert_true(collection_insert(db, &insertQuery).ok);
    assert(db->pagerData->page_id_seq == 7);
    assert(db->pagerData->first_free_document_page_id == 6);
}

void test_queries() {
    print_running("test_queries");
    db_handler* db = open_db_file("tmp");
    test_create_schema(db);
    test_delete_schema(db);
    test_insert_document(db);
    test_insert_document_with_parent(db);
    test_find_all(db);
    test_collection_find(db);
    test_collection_remove(db);
    utilize_db_file(db);
}