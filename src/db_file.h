#ifndef ENORMEDB_DB_FILE_H
#define ENORMEDB_DB_FILE_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "page.h"

// db_file_header: mem info (overall, used, amount of pages allocated), next collection page,
// next collection free page, next string heap page (no need to free because is not deleted) , that's all

typedef struct {
    uint32_t allocated_mem;
    uint32_t used_mem;
    uint32_t amount_of_pages;
} mem_info;

typedef struct {
    mem_info mem_info;
    collection_page* collection_page;
    collection_page* free_collection_page;
    string_heap_page* string_heap;
} db_file_header;

// INTERFACES

int create_db_file(char* db_name);
int delete_db_file(char* db_name);

#endif //ENORMEDB_DB_FILE_H
