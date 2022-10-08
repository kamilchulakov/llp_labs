#ifndef ENORMEDB_PAGER_H
#define ENORMEDB_PAGER_H

#include "db_file.h"
#include "page.h"

int insert_empty_page(db_handler* db_handler);
int get_page_and_debug(db_handler* db_handler, uint32_t page_id);
int create_collection_in_page(db_handler* handler, uint32_t page_id, char* name);
int debug_collection(db_handler* handler, uint32_t page_id);

#endif //ENORMEDB_PAGER_H
