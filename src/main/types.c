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
    if (fread(&str->len, sizeof(int), 1, fp) != 1)
        return READ_ERROR;
    str->ch = malloc(sizeof(char)*(str->len+1));
    str->ch = fgets(str->ch, str->len+1,  fp);
    if (str->ch == NULL)
        return READ_ERROR;
    else
        return READ_OK;
}

WRITE_STATUS write_string(FILE* fp, string* str) {
    if (fwrite(&str->len, sizeof(int), 1, fp) != 1)
        return WRITE_ERROR;
    if (fputs(str->ch, fp) == 1)
        return WRITE_OK;
    else
        return WRITE_ERROR;
}

size_t string_size(string* str) {
    return sizeof(int)+sizeof(char)*str->len;
}