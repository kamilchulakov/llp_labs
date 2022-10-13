#ifndef ENORMEDB_PAGER_H
#define ENORMEDB_PAGER_H

#include "db_file.h"
#include "page.h"
#include "element.h"


int insert_empty_page(db_handler* db_handler);
int get_page_and_debug(db_handler* db_handler, uint32_t page_id);

int create_collection_in_page(db_handler* handler, uint32_t page_id, char* name);
int update_collection_doc_id(db_handler* handler, uint32_t page_id, uint32_t doc_page_id);
int debug_collection(db_handler* handler, uint32_t page_id);

int create_document_in_page(db_handler* handler, uint32_t page_id, uint32_t elements);
int debug_document(db_handler* handler, uint32_t page_id);

#endif //ENORMEDB_PAGER_H
