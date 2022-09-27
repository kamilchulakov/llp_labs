#ifndef ENORMEDB_DOCUMENT_H
#define ENORMEDB_DOCUMENT_H

#include <inttypes.h>

#include "bson.h"

typedef struct document document;

struct document {
    // collection col;
    int32_t bytes;
    element* elements;
};

document* create_document(int32_t bytes, size_t elements);

#endif
