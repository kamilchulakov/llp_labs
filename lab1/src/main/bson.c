#include "io.h"
#include "bson.h"

READ_STATUS read_field(FILE* fp, field* e_field) {
    if (read_string(fp, e_field->e_name) == READ_OK &&
        fread(&e_field->e_type, sizeof(char), 1, fp) == 1) {
        return READ_OK;
    }
    return READ_ERROR;
}

WRITE_STATUS write_field(FILE* fp, field* e_field) {
    if (write_string(fp, e_field->e_name) == WRITE_OK &&
        fwrite(&e_field->e_type, sizeof(char), 1, fp) == 1) {
        return WRITE_OK;
    }
    return WRITE_ERROR;
}

field* empty_field() {
    field* fld = malloc(sizeof(field));
    if (fld == NULL) return NULL;
    fld->e_name = malloc(sizeof(string));
    if (fld->e_name == NULL) {
        free_field(fld);
        return NULL;
    }
    return fld;
}

bool field_equals(field* first, field* second) {
    if (first->e_type != second->e_type) return false;
    if (string_equals(first->e_name, second->e_name) == false) return false;
    return true;
}

void free_field(field* e_field) {
    free_string(e_field->e_name);
    free(e_field);
}