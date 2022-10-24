#include "api.h"
#include "pager.h"

bool collection_fits_filter(collection* col, filter* filter) {
    if (filter->type == ALL) return true;
    if (filter->type == NAME && string_equals(&col->name, filter->data)) return true;
    return false;
}

RESULT select_collection(db_handler* dbHandler, filter* filter) {
    printf("________________");
    printf("\nSELECT QUERY\n");
    if (dbHandler == NULL || dbHandler->fp == NULL ||
        dbHandler->db_file_header == NULL || dbHandler->db_file_header->page_id_seq == 0)
        return RESULT_ERROR;
    page* pg = get_page(dbHandler, 0);
    if (pg == NULL)
        return RESULT_ERROR;
    while (pg->page_header.next_page_id != 0 && pg->page_header.type == PAGE_COLLECTION) {
        collection* col = get_collection(dbHandler, pg->page_header.page_id);
        if (col == NULL)
            return RESULT_ERROR;
        if (collection_fits_filter(col, filter)) debug_collection(col);
        pg = get_page(dbHandler, pg->page_header.next_page_id);
    }
    collection* col = get_collection(dbHandler, pg->page_header.page_id);
    if (col == NULL)
        return RESULT_ERROR;
    debug_collection(col);
    printf("________________");
    return RESULT_OK;
}