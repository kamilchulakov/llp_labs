#include "io.h"
#include "bson.h"

READ_STATUS read_field(FILE* fp, field* e_field) {
    if (fread(&e_field->e_type, sizeof(ELEMENT_TYPE), 1, fp) == 1) {
        return read_string(fp, e_field->e_name);
    }
    return READ_ERROR;
}

WRITE_STATUS write_field(FILE* fp, field* e_field) {
    if (fwrite(&e_field->e_type, sizeof(ELEMENT_TYPE), 1, fp) == 1) {
        return write_string(fp, e_field->e_name);
    }
    return WRITE_ERROR;
}

field* empty_field() {
    field* fld = malloc(sizeof(field));
    if (fld == NULL) return NULL;
    fld->e_name = malloc(sizeof(string));
    if (fld->e_name == NULL) return NULL;
    return fld;
}