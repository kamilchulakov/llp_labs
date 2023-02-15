#include "query_executor.h"
#include "logger.h"
#include "pager.h"

query_result nok() {
    return (query_result) { BOOL_RESULT_TYPE, false};
}

query_result ok() {
    return (query_result) { BOOL_RESULT_TYPE, true};
}

query_result_data* schema_data(schema* sch) {
    query_result_data* data = malloc(sizeof(query_result_data));
    data->type = SCHEMA_RESULT_TYPE;
    data->schema1 = sch;
    return data;
}

query_result schema_result(schema* sch) {
    return (query_result) {DATA_RESULT_TYPE, .data = schema_data(sch)};
}

query_result get_schema_by_name(db_handler* db, string* col) {
    uint32_t collection_page_id = db->db_file_header->first_collection_page_id;
    if (collection_page_id == -1) return nok();
    page* pg = get_page(db, collection_page_id);
    while (pg != NULL) {
        if (!page_has_type(pg, PAGE_COLLECTION)) {
            debug("page(id=%u) is not marked as PAGE_COLLECTION", pg->page_id);
        }
        collection* cl = get_collection(db, pg->page_id);
        if (string_equals(cl->name,col)) return schema_result(cl->sch);
        pg = get_page(db, pg->next_page_id);
    }
    debug("schema %s was not found ", col->ch);
    return nok();
}

query_result get_schema(db_handler* db, get_schema_query* query) {
    debug("GET_SCHEMA: collection=%s\n", query->collection->ch);
    return get_schema_by_name(db, query->collection);
}

query_result create_schema(db_handler* db, create_schema_query* query) {
    debug("CREATE_SCHEMA: collection=%s\n", query->col->name->ch);
    page* pg = get_free_collection_page(db);
    if (write_collection_to_page(db, pg, query->col) == WRITE_OK)
        return ok();
    return nok();
}