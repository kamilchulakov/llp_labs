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

query_result_data* document_list_data(document_list* lst) {
    query_result_data* data = malloc(sizeof(query_result_data));
    data->type = DOCUMENT_LIST_RESULT_TYPE;
    data->documents = lst;
    return data;
}

query_result schema_result(schema* sch) {
    return (query_result) {DATA_RESULT_TYPE, .data = schema_data(sch)};
}

query_result collection_result(collection* col, uint32_t pageId) {
    return (query_result) {DATA_RESULT_TYPE, .data = collection_data(col, pageId)};
}

query_result document_list_result(document_list* lst) {
    return (query_result) {DATA_RESULT_TYPE, .data = document_list_data(lst)};
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
    debug("executor.INSERT_DOCUMENT: (collection=%s, count=%d)\n",
          query->collection->ch, query->doc->data.count);
    document* doc = query->doc;
    query_result res = get_collection_or_schema_by_name(db, query->collection, true);
    if (res.type != DATA_RESULT_TYPE || res.data->type != COLLECTION_RESULT_TYPE) {
        debug("collection not found\n");
        return nok();
    }

    schema* sch = res.data->col->sch;
    if (schema_equals(sch, schema_from_document(doc)) == false) {
        debug("document schema is different from collection schema\n");
        return nok();
    }

    doc->collectionPage = res.data->pageId;

    page* pg;
    if (query->parent == NULL) {
        pg = get_free_document_page(db);
        doc->prevBrotherPage = res.data->col->lastDocPageId;

        if (write_document_to_page_but_split_if_needed(db, pg, doc) == WRITE_OK) {
            res.data->col->lastDocPageId = pg->page_id;
            if (write_collection_to_page(db, res.data->pageId, res.data->col) == WRITE_OK)
                return ok();
        }
    } else {
        document* parentDoc = get_document_header(db, query->parent->parent_id);
        if (parentDoc == NULL) {
            debug("no parent document(id=%d)\n", query->parent->parent_id);
            return nok();
        }

        pg = get_free_document_page(db); // what if allocated, but other things are ruined?
        if (pg == NULL)
            return nok();

        doc->parentPage = query->parent->parent_id;
        doc->prevCollectionDocument = res.data->col->lastDocPageId;
        res.data->col->lastDocPageId = pg->page_id;

        if (write_document_to_page_but_split_if_needed(db, pg, doc) == WRITE_OK &&
                write_collection_to_page(db, res.data->pageId, res.data->col) == WRITE_OK) {
            if (parentDoc->childPage == -1) {
                parentDoc->childPage = pg->page_id;
                if (write_document_header_to_page(db, doc->parentPage, parentDoc) == WRITE_OK)
                    return ok();
            } else {
                uint32_t brotherPage = parentDoc->childPage;
                doc->prevBrotherPage = brotherPage;
                parentDoc->childPage = pg->page_id;

                if (write_document_header_to_page(db, doc->parentPage, parentDoc) == WRITE_OK &&
                    write_document_header_to_page(db, pg->page_id, doc) == WRITE_OK    )
                    return ok();
            }
        }
    }

    return nok();
}

query_result find_all(db_handler* db) {
    if (db->pagerData->lastDocumentPage == -1)
        return nok();
    page* pg = get_page(db, db->pagerData->lastDocumentPage);
    document_list* resList = malloc(sizeof(document_list));
    document_list* currNode = resList;
    while (pg != NULL) {
        if (!page_has_type(pg, PAGE_DOCUMENT)) {
            debug("page(id=%u) is not marked as PAGE_DOCUMENT\n", pg->page_id);
        }

        document* doc = get_document(db, pg->page_id);

        currNode->currDoc = doc;
        currNode->pageId = pg->page_id;
        currNode->nxt = malloc(sizeof(document_list));
        currNode = currNode->nxt;
        pg = get_page(db, pg->prevPageId);
    }

    return document_list_result(resList);
}

query_result collection_find(db_handler* db, find_query* query) {
    if (query->collection == NULL) {
        debug("executor.COLLECTION_FIND without COLLECTION");
        return nok();
    }
    query_result col_res = get_collection_or_schema_by_name(db, query->collection, true);
    if (col_res.type != DATA_RESULT_TYPE || col_res.data->type != COLLECTION_RESULT_TYPE)
        return nok();
    collection* col = col_res.data->col;

    if (col->lastDocPageId == -1)
        return nok();
    page* pg = get_page(db, col->lastDocPageId);
    document_list* resList = malloc(sizeof(document_list));
    document_list* currNode = resList;
    while (pg != NULL) {
        if (!page_has_type(pg, PAGE_DOCUMENT)) {
            debug("page(id=%u) is not marked as PAGE_DOCUMENT\n", pg->page_id);
        }

        document* doc = get_document(db, pg->page_id);

        if (document_satisfies_filter(doc, query->filters) == true) {
            currNode->currDoc = doc;
            currNode->pageId = pg->page_id;
            currNode->nxt = malloc(sizeof(document_list));
            currNode = currNode->nxt;
        }

        pg = get_page(db, doc->prevCollectionDocument);
    }

    return document_list_result(resList);
}

query_result collection_remove(db_handler* db, find_query* query) {
    query_result find_result = collection_find(db, query);
    if (find_result.type != DATA_RESULT_TYPE || find_result.data->type != DOCUMENT_LIST_RESULT_TYPE)
        return nok();
    document_list* curr = find_result.data->documents;
    while (curr->currDoc != NULL) {
        if (remove_document(db, curr->pageId) != WRITE_OK) return nok();
        curr = curr->nxt;
    }
    return ok();
}