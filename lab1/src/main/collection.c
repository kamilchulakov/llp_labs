#include "collection.h"

collection* new_collection(string* name, schema* sch) {
    collection* col = malloc(sizeof(collection));
    if (col == NULL)
        return NULL;
    col->sch = sch;
    col->name = name;
    col->doc_page_id = -1;
    col->lastDocPageId = -1;
    return col;
}

WRITE_STATUS write_collection(FILE* fp, collection* col) {
    if (write_string(fp, col->name) == WRITE_OK &&
        write_uint(fp, &col->doc_page_id) == WRITE_OK &&
        write_uint(fp, &col->lastDocPageId) == WRITE_OK &&
        write_uint(fp, &col->elements_count) == WRITE_OK) {
        return write_schema(fp, col->sch);
    }
    return WRITE_ERROR;
}

READ_STATUS read_collection(FILE* fp, collection* col) {
    if (read_string(fp, col->name) != READ_OK)
        return READ_ERROR;
    if (read_uint(fp, &(col->doc_page_id)) != READ_OK)
        return READ_ERROR;
    if (read_uint(fp, &(col->lastDocPageId)) != READ_OK)
        return READ_ERROR;
    if (read_uint(fp, &(col->elements_count)) != READ_OK)
        return READ_ERROR;
    col->sch = malloc(sizeof(schema));
    if (col->sch == NULL)
        return READ_ERROR;
    return read_schema(fp, col->sch);
}

void free_collection(collection* col) {
    free_string(col->name);
    free_schema(col->sch);
    free(col);
}