#include "pager.h"

size_t calc_page_offset(uint32_t page_id) {
    return sizeof(db_file_header)+PAGE_SIZE*page_id;
}

int insert_empty_page(db_handler* db_handler) {
    page* page = create_empty_page(get_and_set_page_id_seq(db_handler));
    fseek(db_handler->fp, calc_page_offset(page->page_header.page_id), SEEK_SET);
    db_handler->db_file_header->mem_info->allocated_mem += PAGE_SIZE;
    return fwrite(page, sizeof(struct page), 1, db_handler->fp);
}

page* get_page(db_handler* db_handler, uint32_t page_id) {
    page* pg = malloc(sizeof(page));
    fseek(db_handler->fp, calc_page_offset(page_id), SEEK_SET);
    fread(pg, sizeof(page), 1, db_handler->fp);
    return pg;
}

int get_page_and_debug(db_handler* db_handler, uint32_t page_id) {
    return debug_page(get_page(db_handler, page_id));
}

int update_page_header(db_handler* handler, uint32_t page_id, page* pg) {
    fseek(handler->fp, calc_page_offset(page_id), SEEK_SET);
    return fwrite(pg, sizeof(page), 1, handler->fp);
}

int update_page_data(db_handler* handler, uint32_t page_id, void* data, size_t data_size) {
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page_header), SEEK_SET);
    return fwrite(data, data_size, 1, handler->fp);
}

int write_collection_to_page(db_handler* handler, uint32_t page_id, collection* col) {
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page_header), SEEK_SET);
    fwrite(&col->name_size, sizeof(size_t), 1, handler->fp);
    fputs( col->name, handler->fp);
    return update_page_data(handler, page_id, col, strlen(col->name));
}

int create_collection_in_page(db_handler* handler, uint32_t page_id, char* name) {
    collection* col = malloc(sizeof(collection));
    col->name_size = strlen(name);
    col->name = name;
    write_collection_to_page(handler, page_id, col);
    page* pg = create_empty_page(page_id);
    pg->page_header.used_mem = col->name_size+sizeof(size_t);
    pg->page_header.type = PAGE_COLLECTION;
    return update_page_header(handler, page_id, pg);
}

int debug_collection(db_handler* handler, uint32_t page_id) {
    debug_page(get_page(handler, page_id));
    collection* col = malloc(sizeof(collection));
    fread(&(col->name_size), sizeof(size_t), 1, handler->fp);
    col->name = malloc(col->name_size*sizeof(char));
    fgets(col->name, col->name_size+1, handler->fp);
    return printf("COLLECTION:%zu %s", col->name_size, col->name);
}