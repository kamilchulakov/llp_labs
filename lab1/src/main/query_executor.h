#ifndef APP_QUERY_EXECUTOR_H
#define APP_QUERY_EXECUTOR_H

#include "query.h"
#include "db_file.h"

typedef enum { OK, NOK, SCHEMA } result_type;

typedef struct { schema* schemas; } schema_result_data;

typedef union {
    schema_result_data data;
} result_data;

typedef struct {
    result_type type;
    result_data data;
} result;

result execute_schema_query(db_handler* db, schema_query query);

#endif //APP_QUERY_EXECUTOR_H
