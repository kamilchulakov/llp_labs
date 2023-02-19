#ifndef ENORMEDB_DOCUMENT_H
#define ENORMEDB_DOCUMENT_H

#include <inttypes.h>
#include "io.h"
#include "bson.h"
#include "element.h"
#include "schema.h"

typedef struct document {
    uint32_t prevDocInCollectionPage;
    uint32_t elements;
    element* data;
} document;

document* create_document(uint32_t elements);

long size_document_header();

WRITE_STATUS write_document(FILE* fp, document* doc);
WRITE_STATUS write_document_header(FILE* fp, document* doc);
READ_STATUS read_document(FILE* fp, document* doc);
schema* schema_from_document(document* doc);

#endif
