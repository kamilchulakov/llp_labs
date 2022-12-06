#include "../../src/main/query_executor.h"


string example_schema_name();
schema_data example_schema_data();
schema example_schema();

int create_schema(db_handler* db) {
    schema_query create_schema_query = {
        .query_type = CREATE,
        .query_data = {
            .create_query_data = {
                .schema = example_schema()
            }
        }
    };

    execute_schema_query(db, create_schema_query);

    return 0;
}

int scan_schema(db_handler* db) {
    return 0;
}

int delete_schema(db_handler* db) {
    return 0;
}

int main() {
    db_handler* db = open_db_file("example_db");

    create_schema(db);
    scan_schema(db);
    delete_schema(db);

    utilize_db_file(db);

    return 0;
}

string example_schema_name() {
    string str = {.len = 11, .ch = "schema_name"};
    return str;
}

field* example_fields() {
    string str0 = {.len = 3, .ch = "int"};
    string str1 = {.len = 4, .ch = "bool"};
    field* fields = malloc(string_size(&str0) + string_size(&str1) + 2 * sizeof(field));
    fields[0].e_name = str0;
    fields[0].e_type = INT32;
    fields[1].e_name = str1;
    fields[1].e_type = BOOLEAN;

    return fields;
}

schema_data example_schema_data() {
    return (schema_data) { 2, example_fields() };
}

schema example_schema() {
    return (schema) { example_schema_name(), example_schema_data() };
}
