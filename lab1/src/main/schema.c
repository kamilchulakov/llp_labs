#include "schema.h"

WRITE_STATUS write_schema_name(FILE* fp, schema* sch) {
    return write_string(fp, &sch->name);
}

WRITE_STATUS write_schema_data(FILE* fp, schema_data* sch_data) {
    for (int i = 0; i < sch_data->field_count; ++i) {
        if (write_field(fp, sch_data->fields[i]) != WRITE_OK) return WRITE_ERROR;
    }
    return WRITE_OK;
}

WRITE_STATUS write_schema(FILE* fp, schema* sch) {
    if (write_schema_name(fp, sch) == WRITE_OK)
        return write_schema_data(fp, &sch->data);

    return WRITE_ERROR;
}