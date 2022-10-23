#ifndef ENORMEDB_ELEMENT_H
#define ENORMEDB_ELEMENT_H

#include "io.h"
#include "bson.h"

typedef struct {
    field e_field;
    void* e_data;
} element;

element* create_element(ELEMENT_TYPE e_type, char* e_name);
element* create_element_int32(char* e_name, int32_t value);
READ_STATUS read_element(FILE* fp, element* el);
WRITE_STATUS write_element(FILE* fp, element* el);

#endif //ENORMEDB_ELEMENT_H
