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

query_result_data* collection_data(collection* col, uint32_t pageId) {
    query_result_data* data = malloc(sizeof(query_result_data));
    data->type = COLLECTION_RESULT_TYPE;
    data->col = col;
    data->pageId = pageId;
    return data;
}

query_result schema_result(schema* sch) {
    return (query_result) {DATA_RESULT_TYPE, .data = schema_data(sch)};
}

query_result collection_result(collection* col, uint32_t pageId) {
    return (query_result) {DATA_RESULT_TYPE, .data = collection_data(col, pageId)};
}

query_result get_collection_or_schema_by_name(db_handler* db, string* col, bool returnCollection) {
    uint32_t collection_page_id = db->pagerData->lastCollectionPage;
    if (collection_page_id == -1) return nok();
    page* pg = get_page(db, collection_page_id);
    while (pg != NULL) {
        if (!page_has_type(pg, PAGE_COLLECTION)) {
            debug("page(id=%u) is not marked as PAGE_COLLECTION\n", pg->page_id);
        }
        collection* cl = get_collection(db, pg->page_id);
        if (string_equals(cl->name,col) == true) {
            if (returnCollection == true)
                return collection_result(cl, pg->page_id);
            else
                return schema_result(cl->sch);
        }
        pg = get_page(db, pg->prevPageId);
    }
    debug("schema %s was not found\n", col->ch);
    return nok();
}

query_result get_schema(db_handler* db, get_schema_query* query) {
    debug("executor.GET_SCHEMA: collection=%s\n", query->collection->ch);
    return get_collection_or_schema_by_name(db, query->collection, false);
}

query_result create_schema(db_handler* db, create_schema_query* query) {
    debug("executor.CREATE_SCHEMA: collection=%s\n", query->col->name->ch);
    page* pg = get_free_collection_page(db);
    if (write_collection_to_page(db, pg->page_id, query->col) == WRITE_OK)
        return ok();
    return nok();
}

query_result delete_schema(db_handler* db, delete_schema_query* query) {
    debug("executor.DELETE_SCHEMA: collection=%s\n", query->collection->ch);
    query_result res = get_collection_or_schema_by_name(db, query->collection, true);
    if (res.type != DATA_RESULT_TYPE || res.data->type != COLLECTION_RESULT_TYPE) {
        debug("collection not found\n");
        return nok();
    }
    if (res.data->col->elements_count != 0) {
        debug("collection is not empty\n");
        return nok();
    }

    if (free_page(db, res.data->pageId) == WRITE_OK)
        return ok();
    return nok();
}

query_result collection_insert(db_handler* db, insert_query* query) {
    debug("executor.INSERT_DOCUMENT: (collection=%s, elements=%d)\n",
          query->collection->ch, query->doc->elements);
    query_result res = get_collection_or_schema_by_name(db, query->collection, true);
    if (res.type != DATA_RESULT_TYPE || res.data->type != COLLECTION_RESULT_TYPE) {
        debug("collection not found\n");
        return nok();
    }

    schema* sch = res.data->col->sch;
    if (schema_equals(sch, schema_from_document(query->doc)) == false) {
        debug("document schema is different from collection schema\n");
        return nok();
    }

    page* pg = get_free_document_page(db);
    query->doc->prevDocInCollectionPage = res.data->col->lastDocPageId;
    if (write_document_to_page(db, pg, query->doc) == WRITE_OK) {
        res.data->col->lastDocPageId = pg->page_id;
        if (write_collection_to_page(db, res.data->pageId, res.data->col) == WRITE_OK)
            return ok();
    }

    return nok();
}