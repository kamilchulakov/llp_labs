#include "db_file.h"

db_file_header* create_header();
mem_info* create_mem_info();
db_handler* create_db_handler(char* filename, FILE* fp, db_file_header* header);

db_handler* open_db_file(char* db_name) {
    FILE* file = fopen(db_name, "w+");
    if (file == NULL)
        return NULL;
    db_file_header* header = create_header();
    fwrite(header, sizeof(db_file_header), 1, file);
    db_handler* handler = create_db_handler(db_name, file, header);
    return handler;
}

void utilize_db_file(db_handler* db) {
    remove(db->filename);
    free(db->db_file_header->mem_info);
    free(db->db_file_header);
    free(db);
}

db_handler* create_db_handler(char* filename, FILE* fp, db_file_header* header) {
    db_handler* handler = malloc(sizeof(db_handler));
    handler->filename = filename;
    handler->fp = fp;
    handler->db_file_header = header;
}

db_file_header* create_header() {
    db_file_header* header = malloc(sizeof(db_file_header));
    header->mem_info = create_mem_info();
    header->page_id_seq = 0;
    return header;
}

mem_info* create_mem_info() {
    mem_info* mem_info = malloc(sizeof(mem_info));
    mem_info->used_mem = 0;
    mem_info->allocated_mem = 0;
    return mem_info;
}

int debug_mem_info(db_handler* db_handler) {
    return printf("\nAllocated mem:%u\nUsed mem:%u\n", db_handler->db_file_header->mem_info->allocated_mem, db_handler->db_file_header->mem_info->used_mem);
}

uint32_t get_and_set_page_id_seq(db_handler* db_handler) {
    uint32_t curr_page_id = db_handler->db_file_header->page_id_seq;
    db_handler->db_file_header->page_id_seq = curr_page_id+1;
    return curr_page_id;
}