#include "element.h"
#include "types.h"
#include "io.h"
#include <string.h>

element* create_element(ELEMENT_TYPE e_type, char* e_name) {
    element* el = malloc(sizeof(element));
    el->e_field.e_type = e_type;
    el->e_field.e_name.len = strlen(e_name);
    el->e_field.e_name.ch = e_name;
    return el;
}

READ_STATUS read_element_field(FILE* fp, element* el) {
    if (fread(&el->e_field.e_type, sizeof(ELEMENT_TYPE), 1, fp) == 1) {
        return read_string(fp, &el->e_field.e_name);
    }
    return READ_ERROR;
}

WRITE_STATUS write_element_field(FILE* fp, element* el) {
    if (fwrite(&el->e_field.e_type, sizeof(ELEMENT_TYPE), 1, fp) == 1) {
        return write_string(fp, &el->e_field.e_name);
    }
    return WRITE_ERROR;
}

READ_STATUS read_element_data(FILE* fp, element* el) {
    if (el->e_field.e_type == EMPTY)
        return READ_OK;
    if (el->e_field.e_type == BOOLEAN)
        return read_bool(fp, el->e_data);
    if (el->e_field.e_type == INT32 && fread(el->e_data, sizeof(int32_t), 1, fp) == 1)
        return READ_OK;
    return READ_ERROR;
}

WRITE_STATUS write_element_data(FILE* fp, element* el) {
    if (el->e_field.e_type == EMPTY)
        return WRITE_OK;
    if (el->e_field.e_type == BOOLEAN)
        return write_bool(fp, el->e_data);
    if (el->e_field.e_type == INT32 && fwrite(el->e_data, sizeof(int32_t), 1, fp) == 1)
        return WRITE_OK;
    return WRITE_ERROR;
}

WRITE_STATUS write_element(FILE* fp, element* el) {
    if (write_element_field(fp, el) != WRITE_OK)
        return WRITE_ERROR;
    else
        return write_element_data(fp, el);
}

READ_STATUS read_element(FILE* fp, element* el) {
    if (read_element_field(fp, el) != READ_OK)
        return READ_ERROR;
    else
        return read_element_data(fp, el);
}
