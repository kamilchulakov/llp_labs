#include "test_queries.h"
#include "../main/query_executor.h"

test_status test_create_schema(db_handler* db) {
    print_running_test("test_create_schema");
    create_schema_query query = (create_schema_query) {.schema = new_schema(1), .collection = string_of("ex")};
    query.schema->fields[0].e_name = *string_of("my name");
    assert_true(create_schema(db, &query).ok);

    get_schema_query query1 = (get_schema_query) {.collection = string_of("ex")};
    assert(get_schema(db, &query1).data->schema1->field_count == 1);
    return TEST_OK;
}

test_status test_queries() {
    print_running("test_queries");
    db_handler* db = open_db_file("tmp2");
    test_create_schema(db);
    utilize_db_file(db);
    return TEST_OK;
}