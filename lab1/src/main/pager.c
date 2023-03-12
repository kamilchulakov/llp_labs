#include <string.h>
#include "pager.h"
#include "logger.h"

#define MAX_DOCUMENT_DATA_SIZE (size_t) ((PAGE_SIZE - sizeof(page) - sizeof(document)) / sizeof(element))

page* write_page(db_handler *db, const page *pg);

long calc_page_offset(uint32_t page_id) {
    return (long) db_header_size() + PAGE_SIZE * page_id;
}

void set_collection_prev_page(page* pg, db_handler* db) {
    pg->prevPageId = db->pagerData->lastCollectionPage;
    db->pagerData->lastCollectionPage = pg->page_id;
}

void set_document_prev_page(page* pg, db_handler* db) {
    pg->prevPageId = db->pagerData->lastDocumentPage;
    db->pagerData->lastDocumentPage = pg->page_id;
}

void set_string_prev_page(page* pg, db_handler* db) {
    pg->prevPageId = db->pagerData->lastStringPage;
    db->pagerData->lastStringPage = pg->page_id;
}

page* allocate_page_typed(db_handler* db, page_type type) {
    page* pg = create_empty_page(get_and_set_page_id(db));
    debug("pager.ALLOCATE_PAGE: page(id=%u)\n", pg->page_id);
    pg->type = type;
    switch (type) {
        case PAGE_COLLECTION:
            set_collection_prev_page(pg, db);
            break;
        case PAGE_DOCUMENT:
            set_document_prev_page(pg, db);
            break;
        case PAGE_STRING:
            set_string_prev_page(pg, db);
            break;
        default:
            debug("pager.ALLOCATE_PAGE: unsupported page type\n");
            exit(-1);
    }
    return write_page(db, pg);
}

page* write_page(db_handler *db, const page *pg) {
    debug("pager.WRITE_PAGE: page(id=%d)\n", pg->page_id);
    fseek(db->fp, calc_page_offset(pg->page_id), SEEK_SET);
    if (fwrite(pg, sizeof(page), 1, db->fp) == 1)
        return (page*) pg;
    return NULL;
}

page* get_free_collection_page(db_handler* handler) {
    if (handler->pagerData->firstFreeCollectionPageId != -1) {
        debug("pager.GET_FREE_COLLECTION_PAGE: reused page(id=%d)\n", handler->pagerData->firstFreeCollectionPageId);
        page* pg = get_page(handler, handler->pagerData->firstFreeCollectionPageId);
        handler->pagerData->firstFreeCollectionPageId = pg->prevPageId;
        set_collection_prev_page(pg, handler);
        return pg;
    } else {
        page* pg = allocate_page_typed(handler, PAGE_COLLECTION);
        if (pg->prevPageId != -1) {
            page* pgToUpdate = get_page(handler, pg->prevPageId);
            pgToUpdate->nextPageId = pg->page_id;
            if (write_page(handler, pgToUpdate) == NULL)
                return NULL;
        }
        return pg;
    }
}

page* get_free_document_page(db_handler* db) {
    page* pg;
    if (db->pagerData->firstFreeDocumentPageId != -1) {
        debug("pager.GET_FREE_DOCUMENT_PAGE: reused page(id=%d)\n", db->pagerData->firstFreeDocumentPageId);
        pg = get_page(db, db->pagerData->firstFreeDocumentPageId);
        db->pagerData->firstFreeDocumentPageId = pg->prevPageId;
        set_document_prev_page(pg, db);
    } else {
        pg = allocate_page_typed(db, PAGE_DOCUMENT);
    }
    if (pg->prevPageId != -1) {
        page* pgToUpdate = get_page(db, pg->prevPageId);
        pgToUpdate->nextPageId = pg->page_id;
        if (write_page(db, pgToUpdate) == NULL)
            return NULL;
    }

    return write_page(db, pg);
}

page* get_free_string_page(db_handler* db) {
    page* pg;
    if (db->pagerData->firstFreeStringPageId != -1) {
        debug("pager.GET_FREE_STRING_PAGE: reused page(id=%d)\n", db->pagerData->firstFreeStringPageId);
        pg = get_page(db, db->pagerData->firstFreeStringPageId);
        db->pagerData->firstFreeStringPageId = pg->prevPageId;
        set_string_prev_page(pg, db);
    } else {
        pg = allocate_page_typed(db, PAGE_STRING);
    }

    if (pg->prevPageId != -1) {
        page* pgToUpdate = get_page(db, pg->prevPageId);
        pgToUpdate->nextPageId = pg->page_id;
        if (write_page(db, pgToUpdate) == NULL)
            return NULL;
    }

    return write_page(db, pg);
}

page* get_page(db_handler* db_handler, uint32_t page_id) {
    debug("pager.GET_PAGE: page(id=%d)\n", page_id);
    if (page_id < 1) return NULL;
    page* pg = malloc(sizeof(page));
    fseek(db_handler->fp, calc_page_offset(page_id), SEEK_SET);
    if (fread(pg, sizeof(page), 1, db_handler->fp) != 1) return NULL;
    return pg;
}

WRITE_STATUS free_page(db_handler* db, uint32_t page_id) {
    debug("pager.FREE_PAGE: page(id=%d)\n", page_id);
    page* oldPg = get_page(db, page_id);
    page* pg = create_empty_page(page_id);

    switch (oldPg->type) {
        case PAGE_COLLECTION: {
            pg->type = PAGE_COLLECTION;
            if (db->pagerData->firstFreeCollectionPageId != -1) {
                pg->prevPageId = db->pagerData->firstFreeCollectionPageId;
            }
            break;
        }
        case PAGE_DOCUMENT:
            pg->type = PAGE_DOCUMENT;
            if (db->pagerData->firstFreeDocumentPageId != -1) {
                pg->prevPageId = db->pagerData->firstFreeDocumentPageId;
            }
            break;
        case PAGE_STRING:
            pg->type = PAGE_STRING;
            if (db->pagerData->firstFreeStringPageId != -1) {
                pg->prevPageId = db->pagerData->firstFreeStringPageId;
            }
            break;
        default:
            debug("pager.FREE_PAGE: unsupported page type\n");
            exit(-1);
    }

    if (write_page(db, pg) == NULL)
        return WRITE_ERROR;

    switch (oldPg->type) {
        case PAGE_COLLECTION: {
            db->pagerData->firstFreeCollectionPageId = page_id;

            if (db->pagerData->lastCollectionPage == page_id)
                db->pagerData->lastCollectionPage = oldPg->prevPageId;
            break;
        }

        case PAGE_DOCUMENT:
            db->pagerData->firstFreeDocumentPageId = page_id;
            if (db->pagerData->lastDocumentPage == page_id)
                db->pagerData->lastDocumentPage = oldPg->prevPageId;
            break;
        case PAGE_STRING:
            db->pagerData->firstFreeStringPageId = page_id;
            if (db->pagerData->lastStringPage == page_id)
                db->pagerData->lastStringPage = oldPg->prevPageId;
            break;
    }

    if (oldPg->prevPageId != -1) {
        debug("pager.FREE_PAGE: update prev page(id=%d)\n", oldPg->prevPageId);
        page* prevPg = get_page(db, oldPg->prevPageId);
        prevPg->nextPageId = oldPg->nextPageId;
        if (write_page(db, prevPg) == NULL)
            return WRITE_ERROR;

    }
    if (oldPg->nextPageId != -1) {
        debug("pager.FREE_PAGE: update next page(id=%d)\n", oldPg->nextPageId);
        page* nextPg = get_page(db, oldPg->nextPageId);
        nextPg->prevPageId = oldPg->prevPageId;
        if (write_page(db, nextPg) == NULL)
            return WRITE_ERROR;
    }

    return WRITE_OK;
}

WRITE_STATUS write_collection_to_page(db_handler* handler, uint32_t page_id, collection* col) {
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page), SEEK_SET);
    return write_collection(handler->fp, col);
}

WRITE_STATUS write_string_paged(db_handler* db, string_part* part) {
    fseek(db->fp, calc_page_offset(part->pageId)+sizeof(page), SEEK_SET);
    return write_string_split(db->fp, part);
}

READ_STATUS read_string_split_page(db_handler* db, string_part* part) {
    fseek(db->fp, calc_page_offset(part->pageId)+sizeof(page), SEEK_SET);
    return read_string_split(db->fp, part);
}

string_part* read_string_split_by_page(db_handler* db, uint32_t pageId) {
    string_part* part = malloc(sizeof(string_part));
    part->part = string_of_len(0);
    if (part == NULL) return NULL;
    part->pageId = pageId;
    if (read_string_split_page(db, part) != WRITE_OK)
        return NULL;
    return part;
}

READ_STATUS read_string_paged(db_handler* db, string* toRead, string_part* part) {
    if (toRead == NULL || toRead->ch == NULL || toRead->len != part->len) return READ_ERROR;
    string_part* curr = part;
    curr->part = string_of_len(0);
    while (curr->pageId > 0) {
        if (read_string_split_page(db, curr) != READ_OK)
            return READ_ERROR;
        curr->pageId = curr->nxtPageId;
        if (strncat(toRead->ch, curr->part->ch, curr->part->len) == NULL)
            return READ_ERROR;
    }
    return READ_OK;
}

READ_STATUS read_document_strings(db_handler* db, document* doc) {
    for (int i = 0; i < doc->data.count; ++i) {
        element* el = &doc->data.elements[i];
        if (el->e_field->e_type == STRING) {
            if (el->string_split->len == 0) return READ_ERROR;
            string* original = string_of_len(el->string_split->len);
            string_part* curr = el->string_split;
            if (read_string_paged(db, original, curr) != READ_OK)
                return READ_ERROR;
            el->string_data = original;
        }
    }
    return READ_OK;
}

WRITE_STATUS write_document_strings(db_handler* db, document* doc) {
    for (int i = 0; i < doc->data.count; ++i) {
        element* el = &doc->data.elements[i];
        if (el->e_field->e_type == STRING) {
            string_part* curr = el->string_split;
            while (curr) {
                if (write_string_paged(db, curr) == WRITE_ERROR)
                    return WRITE_ERROR;
                curr = curr->nxt;
            }
        }
    }
    return WRITE_OK;
}

void allocate_string_pages(db_handler* db, string_part* split) {
    string_part* curr = split;
    string_part* prev = NULL;
    while (curr) {
        curr->pageId = get_free_string_page(db)->page_id;
        if (prev) prev->nxtPageId = curr->pageId;
        prev = curr;
        curr = curr->nxt;
    }
}

void split_document_strings(db_handler* db, document* doc) {
    for (int i = 0; i < doc->data.count; ++i) {
        element* el = &doc->data.elements[i];
        if (el->e_field->e_type == STRING) {
            size_t len = el->string_data->len;
            el->string_split = split_string(el->string_data, 0);
            el->string_split->len = len;
            allocate_string_pages(db, el->string_split);
        }
    }
}

WRITE_STATUS write_document_to_page(db_handler* db, uint32_t page_id, document* doc) {
    if (doc->data.count > MAX_DOCUMENT_DATA_SIZE) {
        debug("pager.WRITE_DOCUMENT_TO_PAGE: document is too big\n");
        return WRITE_ERROR;
    }

    debug("pager.WRITE_DOCUMENT_TO_PAGE: page(id=%d)\n", page_id);

    split_document_strings(db, doc);
    fseek(db->fp, calc_page_offset(page_id)+sizeof(page), SEEK_SET);
    if (write_document_header(db->fp, doc) != WRITE_OK ||
        write_document_data(db->fp, doc) != WRITE_OK)
        return WRITE_ERROR;
    return write_document_strings(db, doc);
}

WRITE_STATUS write_document_header_to_page(db_handler* db, uint32_t pageId, document* doc) {
    fseek(db->fp, calc_page_offset(pageId) + sizeof(page), SEEK_SET);
    return write_document_header(db->fp, doc);
}

typedef struct pagedData pagedData;
struct pagedData {
    page* pg;
    union {
        document* doc;
    };
};

WRITE_STATUS write_document_to_page_but_split_if_needed(db_handler* db, page* pg, document* doc) {
    if (doc->data.count <= MAX_DOCUMENT_DATA_SIZE)
        return write_document_to_page(db, pg->page_id, doc);

    int count = doc->data.count - MAX_DOCUMENT_DATA_SIZE;
    pagedData prev = { pg, copy_document(doc, 0, MAX_DOCUMENT_DATA_SIZE - 1, true) };
    pagedData curr;
    uint32_t pos = MAX_DOCUMENT_DATA_SIZE;

    while (count > 0) {
        curr.doc = copy_document(doc, pos, pos + MAX_DOCUMENT_DATA_SIZE - 1, false);
        curr.pg = get_free_document_page(db);
        prev.doc->data.nextPage = curr.pg->page_id;
        if (write_document_to_page(db, prev.pg->page_id, prev.doc) != WRITE_OK)
            return WRITE_ERROR;
        prev = curr;
        pos += MAX_DOCUMENT_DATA_SIZE;
        count -= MAX_DOCUMENT_DATA_SIZE;
    }

    return write_document_to_page(db, prev.pg->page_id, prev.doc);
}

collection* empty_collection() {
    collection* col = malloc(sizeof(collection));
    if (col == NULL) return NULL;
    col->name = string_of(NULL);
    return col;
}

collection* get_collection(db_handler* handler, uint32_t page_id) {
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page), SEEK_SET);
    collection* col = empty_collection();
    if (read_collection(handler->fp, col) == READ_ERROR) return NULL;
    return col;
}

document* get_document(db_handler* db, uint32_t page_id) {
    fseek(db->fp, calc_page_offset(page_id)+sizeof(page), SEEK_SET);
    document* doc = malloc(sizeof(document));
    if (read_document(db->fp, doc) == READ_ERROR) return NULL;
    if (read_document_strings(db, doc) != READ_OK)
        return NULL;
    document* curr = doc;
    while (curr->data.nextPage != -1) {
        curr->data.nextDoc = get_document(db, curr->data.nextPage);
        curr = curr->data.nextDoc;
        if (curr == NULL) return NULL;
    }
    return doc;
}

document* get_raw_document(db_handler* db, uint32_t page_id) {
    fseek(db->fp, calc_page_offset(page_id)+sizeof(page), SEEK_SET);
    document* doc = malloc(sizeof(document));
    if (read_document(db->fp, doc) == READ_ERROR) return NULL;
    document* curr = doc;
    while (curr->data.nextPage != -1) {
        curr->data.nextDoc = get_raw_document(db, curr->data.nextPage);
        curr = curr->data.nextDoc;
        if (curr == NULL) return NULL;
    }
    return doc;
}

WRITE_STATUS clear_document_string(db_handler* db, string_part* str) {
    uint32_t currPg = str->pageId;
    string_part* currStr;
    while (currPg > 0) {
        currStr = read_string_split_by_page(db, currPg);
        if (currStr == NULL)
            return WRITE_ERROR;
        if (free_page(db, currPg) != WRITE_OK)
            return WRITE_ERROR;
        currPg = currStr->nxtPageId;
    }
    return WRITE_OK;
}

document* get_document_header(db_handler* db, uint32_t page_id) {
    page* pg = get_page(db, page_id);
    if (pg == NULL) return NULL;
    if (pg->type != PAGE_DOCUMENT) return NULL;
    document *doc = malloc(sizeof(document));
    if (doc == NULL) return NULL;
    if (read_document_header(db->fp, doc) == READ_ERROR) return NULL;
    return doc;
}

WRITE_STATUS clear_all_document_strings(db_handler* db, document* doc) {
    uint32_t count = (uint32_t) doc->data.count;
    for (uint32_t i = 0; i < count; ++i) {
        if (doc->data.elements[i].e_field->e_type == STRING) {
            if (clear_document_string(db, doc->data.elements[i].string_split) != WRITE_OK)
                return WRITE_ERROR;
        }
    }
    return WRITE_OK;
}

WRITE_STATUS remove_document(db_handler* db, uint32_t page_id) {
    document* doc = get_raw_document(db, page_id);
    if (doc == NULL) return WRITE_ERROR;
    if (free_page(db, page_id) != WRITE_OK) return WRITE_ERROR;
    if (clear_all_document_strings(db, doc) != WRITE_OK) return WRITE_ERROR;

    collection* col = get_collection(db, doc->collectionPage);
    if (page_id == col->lastDocPageId) {
        col->lastDocPageId = doc->prevCollectionDocument;
        if (write_collection_to_page(db, doc->collectionPage, col) != WRITE_OK)
            return WRITE_ERROR;
    }

    document* prevCollectionDoc = get_document_header(db, doc->prevCollectionDocument);
    document* nextCollectionDoc = get_document_header(db, doc->nextCollectionDocument);
    if (prevCollectionDoc) {
        prevCollectionDoc->nextCollectionDocument = doc->nextCollectionDocument;
        if (write_document_header_to_page(db, doc->prevCollectionDocument, prevCollectionDoc) != WRITE_OK)
            return WRITE_ERROR;
    }
    if (nextCollectionDoc) {
        nextCollectionDoc->prevCollectionDocument = doc->prevCollectionDocument;
        if (write_document_header_to_page(db, doc->nextCollectionDocument, nextCollectionDoc) != WRITE_OK)
            return WRITE_ERROR;
    }

    document* prevBrother = get_document_header(db, doc->prevBrotherPage);
    document* nextBrother = get_document_header(db, doc->nextBrotherPage);
    if (prevBrother) {
        prevBrother->nextBrotherPage = doc->nextBrotherPage;
        if (write_document_header_to_page(db, doc->prevBrotherPage, prevBrother) != WRITE_OK)
            return WRITE_ERROR;
    }
    if (nextBrother) {
        nextBrother->prevBrotherPage = doc->prevBrotherPage;
        if (write_document_header_to_page(db, doc->nextBrotherPage, nextBrother) != WRITE_OK)
            return WRITE_ERROR;
    }

    document* child = get_document_header(db, doc->childPage);
    while (child) {
        child->parentPage = doc->parentPage;
        if (write_document_header_to_page(db, doc->childPage, child) != WRITE_OK)
            return WRITE_ERROR;
        child = get_document_header(db, child->prevBrotherPage);
    }

    document* part = doc;
    while (part->data.nextPage != -1) {
        if (clear_all_document_strings(db, part) != WRITE_OK)
            return WRITE_ERROR;
        uint32_t pageId = part->data.nextPage;
        part = get_document_header(db, pageId);
        if (free_page(db, pageId) != WRITE_OK)
            return WRITE_ERROR;
    }

    return WRITE_OK;
}

WRITE_STATUS update_raw_document(db_handler* db, uint32_t pageId, element* elements) {
    document* doc = get_raw_document(db, pageId);
    if (doc == NULL || elements == NULL)
        return WRITE_ERROR;
    uint32_t count = (uint32_t) doc->data.count;
    uint32_t sz = (uint32_t) sizeof(*elements) / sizeof(element);
    for (uint32_t i = 0; i < count; ++i) {
        for (uint32_t j = 0; j < sz; ++j) {
            if (field_equals(doc->data.elements[i].e_field, elements[j].e_field) == true) {
                if (elements[j].e_field->e_type == STRING) {
                    if (clear_document_string(db, doc->data.elements[i].string_split) != WRITE_OK)
                        return WRITE_ERROR;
                }
                doc->data.elements[i] = elements[j];
                j++;
            }
        }
    }
    if (doc->data.nextDoc != NULL)
        return update_raw_document(db, doc->data.nextPage, elements);
    return write_document_to_page(db, pageId, doc);
}