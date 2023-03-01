#include <string.h>
#include "types.h"
#include "io.h"
#include "page.h"

#define MAX_STRING_LEN ((size_t) (PAGE_SIZE - sizeof(page) - sizeof(uint32_t)) / sizeof(char))

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
    str->ch = malloc(sizeof(char)*(str->len));
    if (str->ch == NULL)
        return READ_ERROR;
    if (fread(str->ch, sizeof(char), str->len,  fp) != str->len)
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
    return sizeof(size_t)+sizeof(char)*str->len;
}

string* string_of(char* ch) {
    string* str = malloc(sizeof(string));
    if (str == NULL) return NULL;
    if (ch != NULL)
        str->len = strlen(ch);
    str->ch = ch;
    return str;
}

bool string_equals(void* first, void* second) {
    return ((string *) first)->len == ((string *) second)->len && strcmp(((string *) first)->ch, ((string *) second)->ch) == 0;
}

string_part* split_string(string* str, size_t start) {
    if (str == NULL || str->len <= 0) return NULL;
    string_part* part = malloc(sizeof(string_part));
    if (part == NULL) return NULL;

    part->part = malloc(sizeof(string));
    size_t curr_size = MAX_STRING_LEN;
    if (curr_size > str->len) curr_size = str->len;

    part->part->ch = malloc(sizeof(char)*curr_size);
    if (part->part == NULL) return NULL;
    part->part->len = curr_size;
    for (size_t i = 0; i < curr_size; i++) {
        part->part->ch[i] = str->ch[start+i];
    }

    str->len -= curr_size;
    part->nxt = split_string(str, start + curr_size);
    return part;
}

READ_STATUS read_string_split(FILE* fp, string_part *part) {
    if (read_string(fp, part->part) == READ_OK &&
        read_uint(fp, &part->pageId) == READ_OK &&
        read_uint(fp, &part->nxtPageId) == READ_OK)
        return READ_OK;
    return READ_ERROR;
}

READ_STATUS read_string_header_in_document(FILE* fp, string_part *part) {
    return read_uint(fp, &part->pageId);
}

WRITE_STATUS write_string_split(FILE* fp, string_part *part) {
    if (write_string(fp, part->part) == WRITE_OK &&
        write_uint(fp, &part->pageId) == WRITE_OK &&
        write_uint(fp, &part->nxtPageId) == WRITE_OK)
        return WRITE_OK;
    return WRITE_ERROR;
}

WRITE_STATUS write_string_header_in_document(FILE* fp, string_part* part) {
    return write_uint(fp, &part->pageId);
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
