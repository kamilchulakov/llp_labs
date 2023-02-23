#include <string.h>
#include "pager.h"
#include "logger.h"

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
        case PAGE_EMPTY:
            debug("pager.ALLOCATE_PAGE: unsupported page type\n");
            break;
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

WRITE_STATUS write_document_to_page(db_handler* db, page* pg, document* doc) {
    // calc doc size

    // split if needed

    // to write every part
    fseek(db->fp, calc_page_offset(pg->page_id)+sizeof(page), SEEK_SET);
    return write_document(db->fp, doc);
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
    read_collection(handler->fp, col);
    return col;
}

document* get_document(db_handler* handler, uint32_t page_id) {
    fseek(handler->fp, calc_page_offset(page_id)+sizeof(page), SEEK_SET);
    document* doc = malloc(sizeof(document));
    read_document(handler->fp, doc);
    return doc;
}