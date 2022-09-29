#include "db_file.h"

db_file_header* create_header();
mem_info create_mem_info();

int create_db_file(char* db_name) {
    FILE* file = fopen(db_name, "w");
    if (file == NULL) return -1;
    db_file_header* header = create_header();
    fwrite(header, sizeof(db_file_header), 1, file);
    fclose(file);
    return 0;
}

int delete_db_file(char* db_name) {
    return remove(db_name);
}

db_file_header* create_header() {
    db_file_header* header = malloc(sizeof(db_file_header));
    header->mem_info = create_mem_info();
    header->collection_page = NULL;
    header->free_collection_page = NULL;
    header->string_heap = NULL;
    return header;
}

mem_info create_mem_info() {
    mem_info mem_info = {0, 0, 0};
    return mem_info;
}

mem_info read_mem_info(FILE* file) {
    mem_info mem_info = {};
    fread(&mem_info, sizeof(mem_info), 1, file);
    return mem_info;
}