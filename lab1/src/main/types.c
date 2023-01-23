#include <string.h>
#include "types.h"
#include "io.h"

READ_STATUS read_bool(FILE* fp, bool* bl) {
    if (fread(bl, sizeof(bool), 1, fp) == 1)
        return READ_OK;
    else
        return READ_ERROR;
}

WRITE_STATUS write_bool(FILE* fp, bool* bl) {
    if (fwrite(bl, sizeof(bool), 1, fp) == 1)
        return WRITE_OK;
    else
        return WRITE_ERROR;
}

READ_STATUS read_string(FILE* fp, string* str) {
    if (fread(&(str->len), sizeof(size_t), 1, fp) != 1)
        return READ_ERROR;
    str->ch = malloc(sizeof(char)*(str->len+1));
    str->ch = fgets(str->ch, str->len+1,  fp);
    if (str->ch == NULL)
        return READ_ERROR;
    else
        return READ_OK;
}

WRITE_STATUS write_string(FILE* fp, string* str) {
    if (fwrite(&(str->len), sizeof(size_t), 1, fp) != 1)
        return WRITE_ERROR;
    if (fputs(str->ch, fp) == 1)
        return WRITE_OK;
    else
        return WRITE_ERROR;
}

size_t string_size(string* str) {
    return sizeof(int)+sizeof(char)*str->len;
}

string* string_of(char* ch) {
    string* str = malloc(sizeof(string));
    str->len = strlen(ch)+1;
    str->ch = ch;
    return str;
}

bool string_equals(void* first, void* second) {
    return ((string *) first)->len == ((string *) second)->len && strcmp(((string *) first)->ch, ((string *) second)->ch) == 0;
}

READ_STATUS read_uint(FILE* fp, uint32_t* val) {
    if (fread(val, sizeof(uint32_t), 1, fp) == 1)
        return READ_OK;
    return READ_ERROR;
}

WRITE_STATUS write_uint(FILE* fp, uint32_t* val) {
    if (fwrite(val, sizeof(uint32_t), 1, fp) == 1)
        return WRITE_OK;
    return WRITE_ERROR;
}

WRITE_STATUS write_double(FILE* fp, double* val) {
    if (fwrite(val, sizeof(double), 1, fp) == 1)
        return WRITE_OK;
    return WRITE_ERROR;
}

READ_STATUS read_double(FILE* fp, double* val) {
    if (fread(val, sizeof(double), 1, fp) == 1)
        return READ_OK;
    return READ_ERROR;
}
READ_STATUS read_int32(FILE* fp, int32_t* val) {
    if (fread(val, sizeof(int32_t), 1, fp) == 1)
        return READ_OK;
    return READ_ERROR;
}

WRITE_STATUS write_int32(FILE* fp, int32_t* val) {
    if (fwrite(val, sizeof(int32_t), 1, fp) == 1)
        return WRITE_OK;
    return WRITE_ERROR;
}
