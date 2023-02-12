#include "test_queries.h"
#include "../main/query_executor.h"

void test_create_schema(db_handler* db) {
    print_running_test("test_create_schema");

    create_schema_query query = (create_schema_query) { .col = new_collection(string_of("ex"), new_schema(1))};
    query.col->sch->fields[0].e_name = string_of("my name");

    query_result result = create_schema(db, &query);
    assert_result_type(result, BOOL_RESULT_TYPE);
    assert_true(result.ok);

    get_schema_query query1 = (get_schema_query) {.collection = string_of("ex")};
    schema* written_schema = get_schema(db, &query1).data->schema1;
    assert(written_schema->field_count == 1);
    assert_element_equals(written_schema->fields, query.col->sch->fields);
}

void test_queries() {
    print_running("test_queries");
    db_handler* db = open_db_file("tmp");
    test_create_schema(db);
    utilize_db_file(db);
}