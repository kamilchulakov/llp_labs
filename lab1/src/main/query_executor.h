#ifndef APP_QUERY_EXECUTOR_H
#define APP_QUERY_EXECUTOR_H

#include "query.h"
#include "db_file.h"

query_result get_schemas(db_handler* db);
query_result get_schema(db_handler* db, get_schema_query* query);
query_result create_schema(db_handler* db, create_schema_query* query);
query_result delete_schema(db_handler* db, delete_schema_query* query);

query_result collection_find_all(db_handler* db);
query_result collection_find(db_handler* db, find_query* query);
query_result collection_insert(db_handler* db, insert_query* query);
query_result collection_remove(db_handler* db, find_query* query);
query_result collection_update(db_handler* db, update_query* query);

#endif //APP_QUERY_EXECUTOR_H
