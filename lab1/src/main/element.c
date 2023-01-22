#include <string.h>
#include "element.h"
#include "types.h"
#include "io.h"

element* create_element(ELEMENT_TYPE e_type, char* e_name) {
    element* el = malloc(sizeof(element));
    el->e_field = malloc(sizeof(field));
    el->e_field->e_type = e_type;
    el->e_field->e_name.len = strlen(e_name);
    el->e_field->e_name.ch = e_name;
    return el;
}

element* create_element_int32(char* e_name, int32_t value) {
    element* el = create_element(INT32, e_name);
    el->int_data = value;
    return el;
}

READ_STATUS read_element_data(FILE* fp, element* el) {
    if (el->e_field->e_type == BOOLEAN)
        return read_bool(fp, &el->bool_data);
    if (el->e_field->e_type == INT32)
        return read_int32(fp, &el->int_data);
    if (el->e_field->e_type == STRING)
        return read_string(fp, el->string_data);
    if (el->e_field->e_type == DOUBLE)
        return read_double(fp, &el->double_data);
    return READ_ERROR;
}

WRITE_STATUS write_element_data(FILE* fp, element* el) {
    if (el->e_field->e_type == BOOLEAN)
        return write_bool(fp, &el->bool_data);
    if (el->e_field->e_type == INT32)
        return write_int32(fp, &el->int_data);
    if (el->e_field->e_type == STRING)
        return write_string(fp, el->string_data);
    if (el->e_field->e_type == DOUBLE)
        return write_double(fp, &el->double_data);
    return WRITE_ERROR;
}

WRITE_STATUS write_element(FILE* fp, element* el) {
    if (write_field(fp, el->e_field) != WRITE_OK)
        return WRITE_ERROR;
    else
        return write_element_data(fp, el);
}

READ_STATUS read_element(FILE* fp, element* el) {
    if (read_field(fp, el->e_field) != READ_OK)
        return READ_ERROR;
    else
        return read_element_data(fp, el);
}
