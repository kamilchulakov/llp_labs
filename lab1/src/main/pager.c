#include <string.h>
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
    if (fread(pg, sizeof(page), 1, db_handler->fp) != 1) return NULL;
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
    write_string(handler->fp, &col->name);
    fwrite(&col->doc_page_id, sizeof(uint32_t), 1, handler->fp);
    return 0;
}

//int update_collection_doc_id(db_handler* handler, uint32_t page_id, uint32_t doc_page_id) {
//    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page_header), SEEK_SET);
//    collection* col = malloc(sizeof(collection));
//    fread(&(col->name_size), sizeof(size_t), 1, handler->fp);
//    col->name = malloc(col->name_size*sizeof(char));
//    fgets(col->name, col->name_size+1, handler->fp);
//    col->doc_page_id = doc_page_id;
//    write_collection_to_page(handler, page_id, col);
//}

int create_collection_in_page(db_handler* handler, uint32_t page_id, char* name) {
    collection* col = malloc(sizeof(collection));
    col->name.len = strlen(name);
    col->name.ch = name;
    write_collection_to_page(handler, page_id, col);
    page* pg = create_empty_page(page_id);
    pg->page_header.used_mem = string_size(&col->name)+sizeof(uint32_t);
    pg->page_header.type = PAGE_COLLECTION;
    update_page_header(handler, page_id, pg);
    page* prev_pg = get_page(handler, handler->last_collection_page_id);
    prev_pg->page_header.next_page_id = page_id;
    handler->last_collection_page_id = page_id;
    return update_page_header(handler, prev_pg->page_header.page_id, prev_pg);
}

int debug_collection(collection* col) {
    return printf("COLLECTION: NAME_LEN:%d NAME:%s DOC_PAGE_ID:%u\n", col->name.len, col->name.ch, col->doc_page_id);
}

collection* get_collection(db_handler* handler, uint32_t page_id) {
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page_header), SEEK_SET);
    collection* col = malloc(sizeof(collection));
    if (read_string(handler->fp, &col->name) != READ_OK)
        return NULL;
    fread(&(col->doc_page_id), sizeof(uint32_t), 1, handler->fp);
    fread(&(col->last_doc_page_id), sizeof(uint32_t), 1, handler->fp);
    col->sch = malloc(sizeof(schema));
    if (col->sch == NULL || read_schema(handler->fp,  col->sch) == READ_ERROR)
        return NULL;
    return col;
}

document* get_document(db_handler* handler, uint32_t page_id) {
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page_header), SEEK_SET);
    document* doc = malloc(size_document_header());
    read_document(handler->fp, doc);
    return doc;
}

int create_document_in_page(db_handler* handler, uint32_t page_id, uint32_t collection_page_id) {
    document* doc = create_document(0);
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page_header), SEEK_SET);
    write_document(handler->fp, doc);
    page* pg = create_empty_page(page_id);
    pg->page_header.used_mem = size_document_header();
    pg->page_header.type = PAGE_DOCUMENT;
    update_page_header(handler, page_id, pg);
    collection* col = get_collection(handler, collection_page_id);
    uint32_t page_to_update_id = col->last_doc_page_id;
    if (page_to_update_id == 0) page_to_update_id = col->doc_page_id;
    if (col->doc_page_id == 0) col->doc_page_id = page_id;
    col->last_doc_page_id = page_id;
    write_collection_to_page(handler, collection_page_id, col);
    if (page_to_update_id != 0) {
        document *doc_to_update = get_document(handler, page_to_update_id);
        doc_to_update->next_doc_page_id = page_id;
        fseek(handler->fp, calc_page_offset(page_to_update_id) + sizeof(page_header), SEEK_SET);
        return write_document(handler->fp, doc_to_update);
    } else return 0;
}

int debug_document(document* doc) {
    return printf("DOCUMENT: NEXT_DOC_PAGE_ID:%u ELEMENTS:%u\n", doc->next_doc_page_id, doc->elements);
}