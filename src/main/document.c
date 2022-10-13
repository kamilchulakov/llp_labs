#include <stdlib.h>

#include "document.h"

document* create_document(uint32_t elements) {
    document* doc = malloc(sizeof(document));
    if (doc) {
        doc->elements=elements;
        doc->next_doc_page_id=0;
    }
    return doc;
}
