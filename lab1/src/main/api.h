#ifndef ENORMEDB_API_H
#define ENORMEDB_API_H

#include "db_file.h"
#include "types.h"
#include "query.h"

typedef enum {RESULT_OK, RESULT_ERROR} RESULT;

RESULT select_collection(db_handler* dbHandler, filter* filter);
RESULT select_document(db_handler* dbHandler, filter* collection_filter, filter* document_filter);
RESULT insert_element(db_handler* dbHandler, uint32_t document_id, element* el);

#endif //ENORMEDB_API_H
