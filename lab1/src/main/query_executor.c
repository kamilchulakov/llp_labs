#include "query_executor.h"
#include "logger.h"
#include "pager.h"

query_result nok() {
    return (query_result) { BOOL_RESULT_TYPE, false};
}

query_result ok() {
    return (query_result) { BOOL_RESULT_TYPE, true};
}

query_result get_schema_by_name(db_handler* db, string* col) {
    uint32_t collection_page_id = db->db_file_header->first_collection_page_id;
    page* pg = get_page(db, collection_page_id);
    while (pg != NULL) {
        if (!page_has_type(pg, PAGE_COLLECTION)) {
            debug("page(id=%u) is not marked as PAGE_COLLECTION", pg->page_header.page_id);
        }
        collection* cl = get_collection(db, pg->page_header.page_id);
        pg = get_page(db, pg->page_header.next_page_id);
    }
    debug("schema %s was not found ", col->ch);
    return nok();
}

query_result get_schema(db_handler* db, get_schema_query* query) {
    debug("QUERY: get schema");
    return get_schema_by_name(db, query->collection);
}

query_result create_schema(db_handler* db, create_schema_query* query) {
    debug("QUERY: create schema");
    return ok();
}