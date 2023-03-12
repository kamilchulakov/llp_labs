#include "schema.h"

WRITE_STATUS write_schema_data(FILE* fp, schema* sch_data) {
    if (fwrite(sch_data->fields, sizeof(field),
               sch_data->field_count, fp) == sch_data->field_count)
        return WRITE_OK;
    else
        return WRITE_ERROR;
}

WRITE_STATUS write_schema(FILE* fp, schema* sch) {
    if (write_uint(fp, &sch->field_count) == WRITE_OK)
        return write_schema_data(fp, sch);
    return WRITE_ERROR;
}

READ_STATUS read_fields(FILE* fp, schema* sch) {
    sch->fields = malloc(sizeof(field)*sch->field_count);
    if (sch->fields == NULL) return READ_ERROR;

    for (int i = 0; i < sch->field_count; ++i) {
        if (read_field(fp, &sch->fields[i]) == READ_ERROR) {
            return READ_ERROR;
        }
    }

    return READ_OK;
}

READ_STATUS read_schema(FILE* fp, schema* sch) {
    if (read_uint(fp, &sch->field_count) == READ_OK) {
        return read_fields(fp, sch);
    }
    return READ_ERROR;
}

bool schema_equals(schema* first, schema* second) {
    if (first->field_count != second->field_count) return false;
    int* field_is_used = malloc(sizeof(int)*first->field_count);
    if (field_is_used == NULL)
        return false;
    for (size_t i = 0; i < first->field_count; i++) {
        bool find = false;
        for (size_t j = 0; j < first->field_count; j++) {
            if (field_equals(&first->fields[i], &second->fields[j]) == true) {
                if (field_is_used[j] != 1) {
                    field_is_used[j] = 1;
                    find = true;
                    break;
                }
            }
        }
        if (find == false)
            return false;
    }
    return true;
}

schema* new_schema(uint32_t field_count) {
    schema* res = malloc(sizeof(schema));
    if (res == NULL) return NULL;
    res->fields = malloc(sizeof(field)*field_count);
    if (res->fields == NULL) return NULL;
    res->field_count = field_count;
    return res;
}