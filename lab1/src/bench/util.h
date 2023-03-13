#ifndef APP_UTIL_H
#define APP_UTIL_H

#include "../main/query_executor.h"

void insert_schema(db_handler* db);
void insert_documents(db_handler* db, int amount);

void collection_find_all(db_handler* db);
void collection_find_true(db_handler* db);

void clear_all_documents(db_handler* db);

#endif //APP_UTIL_H
