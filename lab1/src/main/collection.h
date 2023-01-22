#ifndef ENORMEDB_COLLECTION_H
#define ENORMEDB_COLLECTION_H

#include "document.h"
#include "schema.h"

typedef struct {
    string name;
    uint32_t doc_page_id;
    uint32_t last_doc_page_id;
    schema* sch;
} collection;

#endif //ENORMEDB_COLLECTION_H
