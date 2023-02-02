#include <string.h>
#include "pager.h"

long calc_page_offset(uint32_t page_id) {
    return (long) db_header_size() + PAGE_SIZE * page_id;
}

page* allocate_page_typed(db_handler* db_handler, page_type type) {
    page* pg = create_empty_page(get_and_set_page_id(db_handler));
    pg->page_header.type = type;
    fseek(db_handler->fp, calc_page_offset(pg->page_header.page_id), SEEK_SET);
    if (fwrite(pg, sizeof(struct page), 1, db_handler->fp) == 1)
        return pg;
    return NULL;
}

page* allocate_page(db_handler* db_handler) {
    page* pg = create_empty_page(get_and_set_page_id(db_handler));
    fseek(db_handler->fp, calc_page_offset(pg->page_header.page_id), SEEK_SET);
    if (fwrite(pg, sizeof(struct page), 1, db_handler->fp) == 1)
        return pg;
    return NULL;
}

page* allocate_collection_page(db_handler* handler) {
    if (handler->db_file_header->first_free_collection_page_id != -1) {
        page* pg = get_page(handler, handler->db_file_header->first_free_collection_page_id);
        handler->db_file_header->first_free_collection_page_id = pg->page_header.next_page_id;
        return pg;
    } else {
        page* pg = allocate_page_typed(handler, PAGE_COLLECTION);
        if (handler->db_file_header->first_collection_page_id == -1) {
            handler->db_file_header->first_collection_page_id = pg->page_header.page_id;
        }
        return pg;
    }
}

page* get_page(db_handler* db_handler, uint32_t page_id) {
    page* pg = malloc(sizeof(page));
    fseek(db_handler->fp, calc_page_offset(page_id), SEEK_SET);
    if (fread(pg, sizeof(page), 1, db_handler->fp) != 1) return NULL;
    return pg;
}

int update_page_header(db_handler* handler, uint32_t page_id, page* pg) {
    fseek(handler->fp, calc_page_offset(page_id), SEEK_SET);
    return fwrite(pg, sizeof(page), 1, handler->fp);
}

int update_page_data(db_handler* handler, uint32_t page_id, void* data, size_t data_size) {
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page_header), SEEK_SET);
    return fwrite(data, data_size, 1, handler->fp);
}

WRITE_STATUS write_collection_to_page(db_handler* handler, page* pg, collection* col) {
    fseek(handler->fp, calc_page_offset(pg->page_header.page_id)+sizeof(page_header), SEEK_SET);
    return write_collection(handler->fp, col);
}

collection* get_collection(db_handler* handler, uint32_t page_id) {
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page_header), SEEK_SET);
    collection* col = malloc(sizeof(collection));
    read_collection(handler->fp, col);
    return col;
}

document* get_document(db_handler* handler, uint32_t page_id) {
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page_header), SEEK_SET);
    document* doc = malloc(size_document_header());
    read_document(handler->fp, doc);
    return doc;
}


int debug_document(document* doc) {
    return printf("DOCUMENT: NEXT_DOC_PAGE_ID:%u ELEMENTS:%u\n", doc->next_doc_page_id, doc->elements);
}