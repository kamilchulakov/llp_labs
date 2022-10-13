#ifndef ENORMEDB_DOCUMENT_H
#define ENORMEDB_DOCUMENT_H

#include <inttypes.h>
#include <bits/types/FILE.h>

#include "bson.h"

typedef struct document document;

struct document {
    uint32_t next_doc_page_id;
    uint32_t elements;
};

document* create_document(uint32_t elements);
int write_document(FILE* fp, document* doc);
int read_document(FILE* fp, document* doc);

#endif
