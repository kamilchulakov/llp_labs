#ifndef ENORMEDB_API_H
#define ENORMEDB_API_H

#include "db_file.h"
#include "types.h"

typedef enum {ALL, ID, NAME} filter_type;

typedef struct {
    filter_type type;
    void* data;
} filter;

typedef enum {RESULT_OK, RESULT_ERROR} RESULT;

RESULT select_collection(db_handler* dbHandler, filter* filter);
RESULT select_document(db_handler* dbHandler, filter* collection_filter, filter* document_filter);

#endif //ENORMEDB_API_H
