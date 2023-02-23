#include <stdlib.h>
#include <glob.h>

#include "document.h"

document* create_document(uint32_t elements) {
    document* doc = malloc(sizeof(document));
    if (doc) {
        doc->parentPage=-1;
        doc->childPage=-1;
        doc->prevBrotherPage=-1;
        doc->brotherPage=-1;
        doc->collectionPage=-1;
        doc->elements=elements;
        doc->data = malloc(sizeof(element)*elements);
    }
    return doc;
}

WRITE_STATUS write_document_header(FILE* fp, document* doc) {
    if (write_uint(fp, &doc->parentPage) == WRITE_OK
        && write_uint(fp, &doc->childPage) == WRITE_OK
        && write_uint(fp, &doc->prevBrotherPage) == WRITE_OK
        && write_uint(fp, &doc->brotherPage) == WRITE_OK
        && write_uint(fp, &doc->collectionPage) == WRITE_OK
        && write_uint(fp, &doc->elements) == WRITE_OK)
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
    if (read_uint(fp, &doc->parentPage) == READ_OK
        && read_uint(fp, &doc->childPage) == READ_OK
        && read_uint(fp, &doc->prevBrotherPage) == READ_OK
        && read_uint(fp, &doc->brotherPage) == READ_OK
        && read_uint(fp, &doc->collectionPage) == READ_OK
        && read_uint(fp, &doc->elements) == READ_OK)
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

schema* schema_from_document(document* doc) {
    schema* sch = new_schema(doc->elements);
    for (size_t i = 0; i < doc->elements; i++) {
        sch->fields[i] = *doc->data[i].e_field;
    }
    return sch;
}

long size_document_header() {
    return sizeof(uint32_t)*2;
}