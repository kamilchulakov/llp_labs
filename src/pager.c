#include "pager.h"

size_t calc_offset(uint32_t page_id) {
    return sizeof(db_file_header)+PAGE_SIZE*page_id;
}

int insert_empty_page(db_handler* db_handler) {
    page* page = create_empty_page(get_and_set_page_id_seq(db_handler));
    fseek(db_handler->fp, calc_offset(page->page_header.page_id), SEEK_SET);
    db_handler->db_file_header->mem_info->allocated_mem += PAGE_SIZE;
    return fwrite(page, sizeof(struct page), 1, db_handler->fp);
}

page* get_page(db_handler* db_handler, uint32_t page_id) {
    page* pg = malloc(sizeof(page));
    fseek(db_handler->fp, calc_offset(page_id), SEEK_SET);
    fread(pg, sizeof(page), 1, db_handler->fp);
    return pg;
}

int get_page_and_debug(db_handler* db_handler, uint32_t page_id) {
    return debug_page(get_page(db_handler, page_id));
}