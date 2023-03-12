#include <stdlib.h>
#include <glob.h>

#include "document.h"

document* create_document(uint32_t elements) {
    document* doc = malloc(sizeof(document));
    if (doc) {
        doc->parentPage=-1;
        doc->childPage=-1;
        doc->prevBrotherPage=-1;
        doc->nextBrotherPage=-1;
        doc->prevCollectionDocument=-1;
        doc->nextCollectionDocument=-1;
        doc->collectionPage=-1;
        doc->data.count=elements;
        doc->data.nextPage=-1;
        doc->data.elements=malloc(sizeof(element)*elements);
    }
    return doc;
}

document* copy_document(document* doc, uint32_t elementsFrom, uint32_t elementsTo, bool withHeader) {
    uint32_t goodElementsTo = elementsTo;
    if (elementsTo >= doc->data.count)
        goodElementsTo = doc->data.count - 1;

    document* res = create_document(goodElementsTo - elementsFrom + 1);
    for (uint32_t i = elementsFrom; i <= goodElementsTo; ++i) {
        res->data.elements[i - elementsFrom] = doc->data.elements[i];
    }
    if (withHeader == true) {
        res->collectionPage =         doc->collectionPage;
        res->prevBrotherPage =         doc->prevBrotherPage;
        res->nextBrotherPage =         doc->nextBrotherPage;
        res->prevCollectionDocument =         doc->prevCollectionDocument;
        res->nextCollectionDocument =  doc->nextCollectionDocument;
        res->childPage =         doc->childPage;
        res->parentPage =         doc->parentPage;
    }
    return res;
}

WRITE_STATUS write_document_header(FILE* fp, document* doc) {
    if (write_uint(fp, &doc->parentPage) == WRITE_OK
        && write_uint(fp, &doc->childPage) == WRITE_OK
        && write_uint(fp, &doc->prevBrotherPage) == WRITE_OK
        && write_uint(fp, &doc->nextBrotherPage) == WRITE_OK
        && write_uint(fp, &doc->prevCollectionDocument) == WRITE_OK
        && write_uint(fp, &doc->nextCollectionDocument) == WRITE_OK
        && write_uint(fp, &doc->collectionPage) == WRITE_OK)
        return WRITE_OK;
    return WRITE_ERROR;
}

WRITE_STATUS write_document_data(FILE* fp, document* doc) {
    if (write_uint(fp, &doc->data.count) != WRITE_OK ||
        write_uint(fp, &doc->data.nextPage) != WRITE_OK)
        return WRITE_ERROR;
    for (int i = 0; i < doc->data.count; ++i) {
        if (write_element(fp, doc->data.elements+i) != WRITE_OK) return WRITE_ERROR;
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
        && read_uint(fp, &doc->nextBrotherPage) == READ_OK
        && read_uint(fp, &doc->prevCollectionDocument) == READ_OK
        && read_uint(fp, &doc->nextCollectionDocument) == READ_OK
        && read_uint(fp, &doc->collectionPage) == READ_OK)
        return READ_OK;
    return READ_ERROR;
}

READ_STATUS read_document_data(FILE* fp, document* doc) {
    if (read_uint(fp, &doc->data.count) != READ_OK ||
        read_uint(fp, &doc->data.nextPage) != READ_OK) return READ_ERROR;
    doc->data.elements = malloc(sizeof(element)*doc->data.count);
    for (int i = 0; i < doc->data.count; ++i) {
        if (read_element(fp, doc->data.elements+i) != READ_OK) return READ_ERROR;
    }
    return READ_OK;
}

READ_STATUS read_document(FILE* fp, document* doc) {
    if (read_document_header(fp, doc) == READ_OK)
        return read_document_data(fp, doc);
    return READ_ERROR;
}

schema* schema_from_document(document* doc) {
    schema* sch = new_schema(doc->data.count);
    for (size_t i = 0; i < doc->data.count; i++) {
        sch->fields[i] = *doc->data.elements[i].e_field;
    }
    return sch;
}

bool document_satisfies_element_filter(document* doc, element_filter* filter) {
    if (doc == NULL) return false;
    element *el = NULL;
    for (int i = 0; i < doc->data.count; ++i) {
        if (field_equals(doc->data.elements[i].e_field, filter->el->e_field) == true) {
            el = doc->data.elements+i;
            break;
        }
    }
    if (el == NULL)
        return false;
    if (cmp_elements(filter->type, el, filter->el) == false)
        return false;
    if (filter->nxt != NULL)
        return document_satisfies_element_filter(doc, filter->nxt);
    return true;
}

bool document_satisfies_operator_filter(document* doc, operator_filter* filter) {
    if (doc == NULL) return false;
    bool first = document_satisfies_filter(doc, filter->flt1);
    if (first == true && filter->type == OR_OPERATOR)
        return true;
    bool second = document_satisfies_filter(doc, filter->flt2);
    return second;
}

bool document_satisfies_filter(document* doc, complex_filter* filter) {
    if (filter == NULL) return true;
    // TODO: test filters
    switch (filter->type) {
        case ELEMENT_FILTER:
            return document_satisfies_element_filter(doc, filter->el_filter);
        case OPERATOR_FILTER:
            return document_satisfies_operator_filter(doc, filter->op_filter);
    }
}
