#include "query_executor.h"

result nok() {
    return (result) {.type = NOK, .data = NULL};
}

result ok() {
    return (result) {.type = OK, .data = NULL};
}


result execute_create_schema_query(db_handler* db, create_schema_query_data* query_data);

result execute_schema_query(db_handler* db, schema_query query) {
    switch (query.query_type) {
        case CREATE:
            return execute_create_schema_query(db, &query.query_data.create_query_data);
        case DELETE:
            return nok();
        case SCAN:
            return nok();
    }
}

result execute_create_schema_query(db_handler* db, create_schema_query_data* query_data) {
    uint32_t page_id = get_and_set_page_id_seq(db);
    // fseek to page
    if (write_schema(db->fp, &query_data->schema) == WRITE_OK)
        return ok();
    return nok();
}