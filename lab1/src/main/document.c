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

WRITE_STATUS write_document_header(FILE* fp, document* doc) {
    if (fwrite(&doc->next_doc_page_id, sizeof(uint32_t), 1, fp) == 1 &&
        fwrite(&doc->elements, sizeof(uint32_t), 1, fp) == 1)
        return WRITE_OK;
    return WRITE_ERROR;
}

WRITE_STATUS write_document_data(FILE* fp, document* doc) {
    element* el = (element* ) doc->data;
    for (int i = 0; i < doc->elements; ++i) {
        if (write_element(fp, &el[i]) != WRITE_OK) return WRITE_ERROR;
    }
    return WRITE_OK;
}

WRITE_STATUS write_document(FILE* fp, document* doc) {
    if (write_document_header(fp, doc) == WRITE_OK)
        return write_document_data(fp, doc);
    return WRITE_ERROR;
}

READ_STATUS read_document_header(FILE* fp, document* doc) {
    if (fread(&doc->next_doc_page_id, sizeof(uint32_t), 1, fp) == 1 &&
            fread(&doc->elements, sizeof(uint32_t), 1, fp) == 1)
        return READ_OK;
    return READ_ERROR;
}

READ_STATUS read_document_data(FILE* fp, document* doc) {
    doc->data = malloc(sizeof(element)*doc->elements);
    element* el = (element* ) doc->data;
    for (int i = 0; i < doc->elements; ++i) {
        if (read_element(fp, &el[i]) != READ_OK) return READ_ERROR;
    }
    return READ_OK;
}

READ_STATUS read_document(FILE* fp, document* doc) {
    if (read_document_header(fp, doc) == READ_OK)
        return read_document_data(fp, doc);
    return READ_ERROR;
}

long size_document_header() {
    return sizeof(uint32_t)*2;
}