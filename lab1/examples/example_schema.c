#include "../src/main/query_executor.h"


schema* example_schema();

int create_schema_example(db_handler* db) {
    create_schema_query query = {
        .schema = example_schema()
    };

    create_schema(db, &query);

    return 0;
}

int scan_schema_example(db_handler* db) {
    return 0;
}

int delete_schema_example(db_handler* db) {
    return 0;
}

int main() {
    db_handler* db = open_db_file("example_db");

    create_schema_example(db);
    scan_schema_example(db);
    delete_schema_example(db);

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

schema* example_schema() {
    schema* sch = malloc(sizeof(schema));
    sch->field_count = 2;
    sch->fields = example_fields();
    return sch;
}
