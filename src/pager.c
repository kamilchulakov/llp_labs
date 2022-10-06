#include "pager.h"

int insert_empty_page(db_handler* db_handler) {
    page* page = create_empty_page(get_and_set_page_id_seq(db_handler));
    db_handler->db_file_header->mem_info->allocated_mem += PAGE_SIZE;
    return fwrite(page, sizeof(struct page), 1, db_handler->fp);
}