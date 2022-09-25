#include <stdlib.h>

#include "document.h"

struct document* create_document(int32_t bytes, size_t elements) {
    document* doc = malloc(sizeof(document));
    if (doc) {
        doc->bytes=bytes;
        doc->elements=malloc(sizeof(element)*elements);
    }
    return doc;
}
