#include "api.h"
#include "pager.h"

//bool collection_fits_filter(collection* col, filter* filter) {
//    if (filter->type == ALL) return true;
//    if (filter->type == NAME && string_equals(&col->name, filter->data)) return true;
//    return false;
//}
//
//bool document_fits_filter(document* doc, filter* filter) {
//    if (filter->type == ALL) return true;
//    return false;
//}
//
//RESULT select_collection(db_handler* dbHandler, filter* filter) {
//    printf("________________\n");
//    printf("SELECT COLLECTION QUERY\n");
//    if (dbHandler == NULL || dbHandler->fp == NULL ||
//        dbHandler->pager_data == NULL || dbHandler->pager_data->page_id_seq == 0)
//        return RESULT_ERROR;
//    page* pg = get_page(dbHandler, 0);
//    if (pg == NULL)
//        return RESULT_ERROR;
//    while (pg->page_header.prevPageId != 0 && pg->page_header.type == PAGE_COLLECTION) {
//        collection* col = get_collection(dbHandler, pg->page_header.page_id);
//        if (col == NULL)
//            return RESULT_ERROR;
//        if (collection_fits_filter(col, filter)) debug_collection(col);
//        pg = get_page(dbHandler, pg->page_header.prevPageId);
//    }
//    collection* col = get_collection(dbHandler, pg->page_header.page_id);
//    if (col == NULL)
//        return RESULT_ERROR;
//    if (collection_fits_filter(col, filter)) debug_collection(col);
//    return RESULT_OK;
//}
//
//typedef struct {
//    page* pg;
//    collection* col;
//} page_with_collection;
//
//page_with_collection* find_page_with_collection(db_handler *handler, filter *filter) {
//    if (filter->type != ID)
//        return NULL;
//    page_with_collection* pg_with_col = malloc(sizeof(page_with_collection));
//    pg_with_col->pg = get_page(handler, *(uint32_t*)filter->data);
//    pg_with_col->col = get_collection(handler, pg_with_col->pg->page_header.page_id);
//    return pg_with_col;
//}
//
//RESULT select_document(db_handler* dbHandler, filter* collection_filter, filter* document_filter) {
//    printf("________________\n");
//    printf("SELECT DOCUMENT QUERY\n");
//    if (dbHandler == NULL || dbHandler->fp == NULL ||
//        dbHandler->pager_data == NULL || dbHandler->pager_data->page_id_seq == 0)
//        return RESULT_ERROR;
//    page_with_collection* pg_with_col = find_page_with_collection(dbHandler, collection_filter);
//    if (pg_with_col == NULL || pg_with_col->pg == NULL)
//        return RESULT_ERROR;
//    page* pg = get_page(dbHandler, pg_with_col->col->doc_page_id);
//    document* doc = get_document(dbHandler, pg->page_header.page_id);
//    while (pg->page_header.type == PAGE_DOCUMENT && doc->next_doc_page_id != 0) {
//        if (document_fits_filter(doc, document_filter)) debug_document(doc);
//        pg = get_page(dbHandler, doc->next_doc_page_id);
//        doc = get_document(dbHandler, doc->next_doc_page_id);
//        if (doc == NULL)
//            return RESULT_ERROR;
//    }
//    doc = get_document(dbHandler, pg->page_header.page_id);
//    if (doc == NULL)
//        return RESULT_ERROR;
//    if (document_fits_filter(doc, document_filter)) debug_document(doc);
//    return RESULT_OK;
//}
//
//int update_mem(db_handler* handler, page* pg, uint32_t mem) {
//    pg->page_header.used_mem += mem;
//    pg->page_header.last_modified = time(NULL);
//    return update_page_header(handler, pg->page_header.page_id, pg);
//}
//
//
//RESULT insert_element(db_handler* dbHandler, uint32_t document_id, element* el) {
//    page* pg = get_page(dbHandler, document_id);
//    update_mem(dbHandler, pg, sizeof(field));
//    document* doc = get_document(dbHandler, document_id);
//    doc->elements_count += 1;
//    write_document_header(dbHandler->fp, doc);
//    if (write_element(dbHandler->fp, el) == WRITE_OK) return RESULT_OK;
//    return RESULT_ERROR;
//}