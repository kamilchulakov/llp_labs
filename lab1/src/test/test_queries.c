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

    insert_query insertQuery = {0, string_of("ex"), create_document(1)};
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

    collection* col = get_collection(db, 1);
    assert(col->lastDocPageId == 2);
    assert(col->doc_page_id == -1);
}

void test_queries() {
    print_running("test_queries");
    db_handler* db = open_db_file("tmp");
    test_create_schema(db);
    test_delete_schema(db);
    test_insert_document(db);
    utilize_db_file(db);
}