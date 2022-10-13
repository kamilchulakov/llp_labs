#include "element.h"
#include "bool.h"
#include "stdlib.h"

element* create_element(ELEMENT_TYPE e_type, char* e_name) {
    element* el = malloc(sizeof(element));
    el->e_field.e_type = e_type;
    el->e_field.e_name = e_name;
    return el;
}


READ_STATUS read_field_data(FILE* fp, element* el) {
    if (el->e_field.e_type == EMPTY)
        return READ_OK;
    if (el->e_field.e_type == BOOLEAN)
        return read_bool(fp, el->e_data);
    return READ_ERROR;
}

WRITE_STATUS write_element(FILE* fp, element* el) {
    if (fwrite(el, sizeof(element), 1, fp) != 1)
        return WRITE_ERROR;
    else
        return WRITE_OK;
}

READ_STATUS read_element(FILE* fp, element* el) {
    if (fread(&el->e_field, sizeof(field), 1, fp) != 1)
        return READ_ERROR;
    else
        return read_field_data(fp, el);
}
