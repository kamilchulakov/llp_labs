#ifndef ENORMEDB_PAGE_H
#define ENORMEDB_PAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "collection.h"
#include "db_file.h"

#define PAGE_SIZE 4096

typedef struct page page;
typedef enum { PAGE_COLLECTION, PAGE_DOCUMENT, PAGE_EMPTY } page_type;
typedef enum { false, true } bool;

typedef struct {
    uint32_t page_id;
    bool is_full;
    uint32_t next_page_id;
    time_t last_modified;
    uint32_t used_mem; // in bytes, of PAGE_SIZE
    page_type type;
} page_header;

struct page {
    page_header page_header;
};

page* create_empty_page(uint32_t page_id);
int debug_page(page* page);

#endif //ENORMEDB_PAGE_H
