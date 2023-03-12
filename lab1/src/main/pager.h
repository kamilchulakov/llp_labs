#ifndef ENORMEDB_PAGER_H
#define ENORMEDB_PAGER_H

#include "db_file.h"
#include "page.h"
#include "element.h"


page* allocate_page_typed(db_handler* db, page_type type);
page* get_free_collection_page(db_handler* handler);
page* get_free_document_page(db_handler* db);
page* get_page(db_handler* db_handler, uint32_t page_id);
WRITE_STATUS free_page(db_handler* db, uint32_t page_id);

WRITE_STATUS remove_document(db_handler* db, uint32_t page_id);

WRITE_STATUS write_collection_to_page(db_handler* handler, uint32_t page_id, collection* col);
WRITE_STATUS write_document_to_page(db_handler* db, uint32_t page_id, document* doc);
WRITE_STATUS write_document_to_page_but_split_if_needed(db_handler* db, page* pg, document* doc);
WRITE_STATUS write_document_header_to_page(db_handler* db, uint32_t pageId, document* doc);

WRITE_STATUS update_raw_document(db_handler* db, uint32_t pageId, element* elements);

collection* get_collection(db_handler* handler, uint32_t page_id);
document* get_document(db_handler* db, uint32_t page_id);
document* get_document_header(db_handler* db, uint32_t page_id);

#endif //ENORMEDB_PAGER_H
