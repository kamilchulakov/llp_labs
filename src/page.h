#ifndef ENORMEDB_PAGE_H
#define ENORMEDB_PAGE_H

#include "collection.h"
#include "string_heap.h"

#define PAGE_SIZE 4096

typedef uint64_t datetime;
typedef struct page page;

struct page {
    page* prev_page;
    page* next_page;
    uint32_t page_id;
    datetime last_modified;
};

typedef struct {
    page* page;
    collection* collection;
} collection_page;

typedef struct {
    page* page;
    string_heap* string_heap;
} string_heap_page;

#endif //ENORMEDB_PAGE_H
