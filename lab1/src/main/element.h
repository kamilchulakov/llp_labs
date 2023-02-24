#ifndef ENORMEDB_ELEMENT_H
#define ENORMEDB_ELEMENT_H

#include "io.h"
#include "bson.h"

typedef struct {
    field* e_field;
    union {
        int32_t int_data;
        double double_data;
        bool bool_data;
        string* string_data;
        string_part* string_split;
    };
} element;

typedef enum { OP_EQUAL, OP_NOT_EQUAL, OP_GREATER, OP_LESS } operator_type;

typedef struct {
    operator_type type;
    element* element;
} element_filter;

element* create_element(ELEMENT_TYPE e_type, char* e_name);
element* create_element_int32(char* e_name, int32_t value);
READ_STATUS read_element(FILE* fp, element* el);
WRITE_STATUS write_element(FILE* fp, element* el);

#endif //ENORMEDB_ELEMENT_H
