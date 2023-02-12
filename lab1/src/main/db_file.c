#include "db_file.h"
#include "logger.h"

db_file_header* create_header();
mem_info* create_mem_info();
db_handler* create_db_handler(char* filename, FILE* fp, db_file_header* header);
WRITE_STATUS write_db_header(FILE* fp, db_file_header* header);

size_t db_header_size() {
    return sizeof(db_file_header);
}

db_handler* open_db_file(char* db_name) {
    FILE* file = fopen(db_name, "w+");
    if (file == NULL)
        return NULL;
    db_file_header* header = create_header();

    db_handler* handler = create_db_handler(db_name, file, header);
    return handler;
}

void utilize_db_file(db_handler* db) {
    remove(db->filename);
    free(db->db_file_header->mem);
    free(db->db_file_header);
    free(db);
}

void seek_db_header(FILE* fp) {
    fseek(fp, 0, SEEK_SET);
}

db_handler* create_db_handler(char* filename, FILE* fp, db_file_header* header) {
    db_handler* handler = malloc(sizeof(db_handler));
    handler->filename = filename;
    handler->fp = fp;
    handler->db_file_header = header;
    return handler;
}

db_file_header* create_header() {
    db_file_header* header = malloc(sizeof(db_file_header));
    header->mem = create_mem_info();
    header->page_id_seq = 1;
    header->first_collection_page_id = -1;
    header->first_free_collection_page_id = -1;
    return header;
}

WRITE_STATUS write_db_header(FILE* fp, db_file_header* header) {
    seek_db_header(fp);
    if (fwrite( header, sizeof(db_file_header), 1, fp) == 1)
        return WRITE_OK;
    return WRITE_ERROR;
}

mem_info* create_mem_info() {
    mem_info* mem = malloc(sizeof(mem_info));
    mem->used_mem = 0;
    mem->allocated_mem = 0;
    return mem;
}

int debug_mem_info(db_handler* db_handler) {
    return printf("\nAllocated mem:%u\nUsed mem:%u\n", db_handler->db_file_header->mem->allocated_mem, db_handler->db_file_header->mem->used_mem);
}

uint32_t get_and_set_page_id(db_handler* db_handler) {
    uint32_t curr_page_id = db_handler->db_file_header->page_id_seq;
    db_handler->db_file_header->page_id_seq = curr_page_id+1;
    db_handler->db_file_header->mem->allocated_mem += PAGE_SIZE;
    write_db_header(db_handler->fp, db_handler->db_file_header);
    return curr_page_id;
}