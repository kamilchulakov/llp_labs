#ifndef ENORMEDB_PAGE_H
#define ENORMEDB_PAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "collection.h"
#include "db_file.h"
#include "types.h"

#define PAGE_SIZE 256

typedef enum { PAGE_COLLECTION, PAGE_DOCUMENT, PAGE_STRING, PAGE_EMPTY } page_type;

typedef struct {
    uint32_t page_id;
    bool is_full;
    uint32_t prevPageId;
    uint32_t nextPageId;
    time_t last_modified;
    uint32_t used_mem; // in bytes, of PAGE_SIZE
    page_type type;
} page;

page* create_empty_page(uint32_t page_id);
int debug_page(page* page);
bool page_has_type(page* page, page_type);

#endif //ENORMEDB_PAGE_H
