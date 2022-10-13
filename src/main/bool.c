#include "bool.h"
#include "inttypes.h"
typedef int8_t BOOL;

READ_STATUS read_bool(FILE* fp, bool* bl) {
    if (fread(bl, sizeof(BOOL), 1, fp) == 1)
        return READ_OK;
    else
        return READ_ERROR;
}
WRITE_STATUS write_bool(FILE* fp, bool* bl) {
    if (fread(bl, sizeof(BOOL), 1, fp) == 1)
        return WRITE_OK;
    else
        return WRITE_ERROR;
}