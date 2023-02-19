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
int get_page_and_debug(db_handler* db_handler, uint32_t page_id);
int update_page_header(db_handler* handler, uint32_t page_id, page* pg);

WRITE_STATUS write_collection_to_page(db_handler* handler, uint32_t page_id, collection* col);
WRITE_STATUS write_document_to_page(db_handler* db, page* pg, document* doc);

int update_collection_doc_id(db_handler* handler, uint32_t page_id, uint32_t doc_page_id);
collection* get_collection(db_handler* handler, uint32_t page_id);
int debug_collection(collection* co);

int create_document_in_page(db_handler* handler, uint32_t page_id, uint32_t collection_page_id);
document* get_document(db_handler* handler, uint32_t page_id);
int debug_document(document* doc);

#endif //ENORMEDB_PAGER_H
