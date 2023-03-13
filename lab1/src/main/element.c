#include <string.h>
#include "element.h"
#include "types.h"
#include "io.h"

element* create_element(ELEMENT_TYPE e_type, char* e_name) {
    element* el = malloc(sizeof(element));
    el->e_field = malloc(sizeof(field));
    el->e_field->e_type = e_type;
    el->e_field->e_name = string_of(e_name);
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
    if (el->e_field->e_type == STRING) {
        el->string_split = malloc(sizeof(string_part));
        if (el->string_split == NULL) return READ_ERROR;
        return read_string_header_in_document(fp, el->string_split);
    }
    if (el->e_field->e_type == DOUBLE)
        return read_double(fp, &el->double_data);
    return READ_ERROR;
}

WRITE_STATUS write_element_data(FILE* fp, element* el) {
    if (el->e_field->e_type == BOOLEAN)
        return write_bool(fp, &el->bool_data);
    if (el->e_field->e_type == INT32)
        return write_int32(fp, &el->int_data);
    if (el->e_field->e_type == DOUBLE)
        return write_double(fp, &el->double_data);
    if (el->e_field->e_type == STRING)
        return write_string_header_in_document(fp, el->string_split); // head
    return WRITE_ERROR;
}

WRITE_STATUS write_element(FILE* fp, element* el) {
    if (write_field(fp, el->e_field) != WRITE_OK)
        return WRITE_ERROR;
    else
        return write_element_data(fp, el);
}

READ_STATUS read_element(FILE* fp, element* el) {
    el->e_field = empty_field();
    if (el->e_field == NULL) return READ_ERROR;
    if (read_field(fp, el->e_field) != READ_OK)
        return READ_ERROR;
    else
        return read_element_data(fp, el);
}

element_filter* create_element_filter(
        CMP_TYPE op_type,
        element* el) {
    element_filter* flt = malloc(sizeof(element_filter));
    if (flt == NULL) return NULL;
    flt->type = op_type;
    flt->el = el;
    flt->nxt = NULL;
    return flt;
}

element_filter* create_element_filter_linked(
        CMP_TYPE op_type,
        element* el,
        element_filter* prev,
        element_filter* nxt) {
    element_filter* flt = create_element_filter(op_type, el);
    if (flt != NULL) flt->nxt = nxt;
    if (prev != NULL) prev->nxt = flt;
    return flt;
}

bool cmp_elements(CMP_TYPE cmp_type, element* first, element* second) {
    switch (cmp_type) {
        case CMP_EQ:
            switch (first->e_field->e_type) {
                case DOUBLE:
                case BOOLEAN:
                case INT32:
                    if (first->int_data == second->int_data)
                        return true;
                    else
                        return false;
                case STRING:
                    return string_equals(first->string_data, second->string_data);
            }
        case CMP_NEQ:
            switch (first->e_field->e_type) {
                case DOUBLE:
                case BOOLEAN:
                case INT32:
                    if (first->int_data != second->int_data)
                        return true;
                    else
                        return false;
                case STRING:
                    if (string_equals(first->string_data, second->string_data) == false)
                        return true;
                    else return false;
            }
        case CMP_GT:
            switch (first->e_field->e_type) {
                case DOUBLE:
                    if (first->double_data > second->double_data) return true;
                    return false;
                case BOOLEAN:
                case INT32:
                    if (first->int_data > second->int_data)
                        return true;
                    return false;
                case STRING:
                    if (strcmp(first->string_data->ch, second->string_data->ch) > 0)
                        return true;
                    return false;
            }
        case CMP_GTE:
            switch (first->e_field->e_type) {
                case DOUBLE:
                    if (first->double_data >= second->double_data) return true;
                    return false;
                case BOOLEAN:
                case INT32:
                    if (first->int_data >= second->int_data)
                        return true;
                    return false;
                case STRING:
                    if (strcmp(first->string_data->ch, second->string_data->ch) >= 0)
                        return true;
                    return false;
            }
        case CMP_LT:
            switch (first->e_field->e_type) {
                case DOUBLE:
                    if (first->double_data < second->double_data) return true;
                    return false;
                case BOOLEAN:
                case INT32:
                    if (first->int_data < second->int_data)
                        return true;
                    return false;
                case STRING:
                    if (strcmp(first->string_data->ch, second->string_data->ch) < 0)
                        return true;
                    return false;
            }
        case CMP_LTE:
            switch (first->e_field->e_type) {
                case DOUBLE:
                    if (first->double_data <= second->double_data) return true;
                    return false;
                case BOOLEAN:
                case INT32:
                    if (first->int_data <= second->int_data)
                        return true;
                    return false;
                case STRING:
                    if (strcmp(first->string_data->ch, second->string_data->ch) <= 0)
                        return true;
                    return false;
            }
        case CMP_REGEX:
			break;
    }
    return false;
}