#ifndef ENORMEDB_DOCUMENT_H
#define ENORMEDB_DOCUMENT_H

#include <inttypes.h>
#include <bits/types/FILE.h>

#include "bson.h"

typedef struct document document;

struct document {
    uint32_t next_doc_page_id;
    uint32_t elements;
    void* data;
};

document* create_document(uint32_t elements);
WRITE_STATUS write_document(FILE* fp, document* doc);
READ_STATUS read_document(FILE* fp, document* doc);

#endif
