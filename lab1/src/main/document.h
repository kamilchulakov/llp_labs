#ifndef ENORMEDB_DOCUMENT_H
#define ENORMEDB_DOCUMENT_H

#include <inttypes.h>
#include "io.h"
#include "bson.h"
#include "element.h"
#include "schema.h"

typedef struct {
    uint32_t parentPage;
    uint32_t childPage;
    uint32_t prevBrotherPage;
    uint32_t prevCollectionDocument;
    uint32_t collectionPage;

    struct document_data {
        uint32_t nextPage;
        uint32_t count;
        element* elements;
    } data;
} document;

document* create_document(uint32_t elements);
document* copy_document(document* doc, uint32_t elementsFrom, uint32_t elementsTo, bool withHeader);
size_t document_size(document* doc);

WRITE_STATUS write_document(FILE* fp, document* doc);
WRITE_STATUS write_document_data(FILE* fp, document* doc);
WRITE_STATUS write_document_header(FILE* fp, document* doc);
READ_STATUS read_document_header(FILE* fp, document* doc);
READ_STATUS read_document(FILE* fp, document* doc);
schema* schema_from_document(document* doc);

bool document_satisfies_filter(document* doc, complex_filter* filter);

#endif
