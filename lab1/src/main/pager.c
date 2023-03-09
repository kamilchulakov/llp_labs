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
    if (handler->pagerData->first_free_collection_page_id != -1) {
        debug("pager.GET_FREE_COLLECTION_PAGE: reused page(id=%d)\n", handler->pagerData->first_free_collection_page_id);
        page* pg = get_page(handler, handler->pagerData->first_free_collection_page_id);
        handler->pagerData->first_free_collection_page_id = pg->prevPageId;
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
    if (db->pagerData->first_free_document_page_id != -1) {
        debug("pager.GET_FREE_DOCUMENT_PAGE: reused page(id=%d)\n", db->pagerData->first_free_document_page_id);
        page* pg = get_page(db, db->pagerData->first_free_document_page_id);
        db->pagerData->first_free_document_page_id = pg->prevPageId;
        set_document_prev_page(pg, db);
        return pg;
    } else {
        page* pg = allocate_page_typed(db, PAGE_DOCUMENT);
        if (pg->prevPageId != -1) {
            page* pgToUpdate = get_page(db, pg->prevPageId);
            pgToUpdate->nextPageId = pg->page_id;
            if (write_page(db, pgToUpdate) == NULL)
                return NULL;
        }
        return pg;
    }
}

page* get_page(db_handler* db_handler, uint32_t page_id) {
    debug("pager.GET_PAGE: page(id=%d)\n", page_id);
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
            if (db->pagerData->first_free_collection_page_id != -1) {
                pg->prevPageId = db->pagerData->first_free_collection_page_id;
            }
        }
        case PAGE_DOCUMENT:
            break;
        case PAGE_EMPTY:
            break;
    }

    if (write_page(db, pg) == NULL)
        return WRITE_ERROR;

    switch (oldPg->type) {
        case PAGE_COLLECTION: {
            db->pagerData->first_free_collection_page_id = page_id;

            if (db->pagerData->lastCollectionPage == page_id)
                db->pagerData->lastCollectionPage = -1;
        }

        case PAGE_DOCUMENT:
            break;
        case PAGE_EMPTY:
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
        curr->pageId = allocate_page_typed(db, PAGE_STRING)->page_id;
        if (prev) prev->nxtPageId = curr->pageId;
        prev = curr;
        curr = curr->nxt;
    }
}

void split_document_strings(db_handler* db, document* doc) {
    for (int i = 0; i < doc->data.count; ++i) {
        element* el = &doc->data.elements[i];
        if (el->e_field->e_type == STRING) {
            el->string_split = split_string(el->string_data, 0);
            allocate_string_pages(db, el->string_split);
        }
    }
}

WRITE_STATUS write_document_to_page(db_handler* db, page* pg, document* doc) {
    if (doc->data.count > MAX_DOCUMENT_DATA_SIZE) {
        debug("pager.WRITE_DOCUMENT_TO_PAGE: document is too big\n");
        return WRITE_ERROR;
    }

    debug("pager.WRITE_DOCUMENT_TO_PAGE: page(id=%d)\n", pg->page_id);

    split_document_strings(db, doc);
    fseek(db->fp, calc_page_offset(pg->page_id)+sizeof(page), SEEK_SET);
    if (write_document_header(db->fp, doc) != WRITE_OK ||
        write_document_data(db->fp, doc) != WRITE_OK)
        return WRITE_ERROR;
    return write_document_strings(db, doc);
}

WRITE_STATUS write_document_header_to_page(db_handler* db, uint32_t pageId, document* doc) {
    fseek(db->fp, calc_page_offset(pageId) + sizeof(page), SEEK_SET);
    return write_document_header(db->fp, doc);
}

typedef struct {
    page* pg;
    union {
        document* doc;
    };
} pagedData;

WRITE_STATUS write_document_to_page_but_split_if_needed(db_handler* db, page* pg, document* doc) {
    if (doc->data.count <= MAX_DOCUMENT_DATA_SIZE)
        return write_document_to_page(db, pg, doc);


    int count = doc->data.count - MAX_DOCUMENT_DATA_SIZE;
    pagedData prev = { pg, copy_document(doc, 0, MAX_DOCUMENT_DATA_SIZE - 1, true) };
    pagedData curr;
    uint32_t pos = MAX_DOCUMENT_DATA_SIZE;

    while (count > 0) {
        curr.doc = copy_document(doc, pos, pos + MAX_DOCUMENT_DATA_SIZE - 1, false);
        curr.pg = get_free_document_page(db);
        prev.doc->data.nextPage = curr.pg->page_id;
        if (write_document_to_page(db, prev.pg, prev.doc) != WRITE_OK)
            return WRITE_ERROR;
        prev = curr;
        pos += MAX_DOCUMENT_DATA_SIZE;
        count -= MAX_DOCUMENT_DATA_SIZE;
    }

    return write_document_to_page(db, prev.pg, prev.doc);
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

document* get_document(db_handler* handler, uint32_t page_id) {
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page), SEEK_SET);
    document* doc = malloc(sizeof(document));
    if (read_document(handler->fp, doc) == READ_ERROR) return NULL;
    return doc;
}

document* get_document_header(db_handler* db, uint32_t page_id) {
    page* pg = get_page(db, page_id);
    if (pg->type != PAGE_DOCUMENT) return NULL;
    document *doc = malloc(sizeof(document));
    if (doc == NULL) return NULL;
    if (read_document_header(db->fp, doc) == READ_ERROR) return NULL;
    return doc;
}