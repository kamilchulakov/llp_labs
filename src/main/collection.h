#ifndef ENORMEDB_COLLECTION_H
#define ENORMEDB_COLLECTION_H

#include "document.h"

typedef struct {
    size_t name_size;
    char* name;
    uint32_t doc_page_id;
} collection;

#endif //ENORMEDB_COLLECTION_H
