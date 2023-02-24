#ifndef ENORMEDB_DB_FILE_H
#define ENORMEDB_DB_FILE_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "page.h"

// pager_data: mem info (overall, used, amount of pages allocated), next collection page,
// next collection free page, next string heap page (no need to free because is not deleted) , that's all

typedef struct {
    uint32_t allocated_mem;
    uint32_t used_mem;
} mem_info;

typedef struct {
    mem_info* mem;
    uint32_t page_id_seq; // page that can be allocated

    uint32_t lastCollectionPage;
    uint32_t first_free_collection_page_id; // page that is already allocated

    uint32_t lastDocumentPage;
    uint32_t first_free_document_page_id; // page that is already allocated

    uint32_t lastStringPage;
    uint32_t first_free_string_page_id; // page that is already allocated
} pager_data;

typedef struct {
    char* filename;
    FILE* fp;
    pager_data* pagerData;
    uint32_t last_collection_page_id;
} db_handler;

// INTERFACES

size_t db_header_size();

db_handler* open_db_file(char* db_name);
void utilize_db_file(db_handler* db);
void seek_db_header(FILE* fp);

uint32_t get_and_set_page_id(db_handler* db_handler);
int debug_mem_info(db_handler* db_handler);

#endif //ENORMEDB_DB_FILE_H
