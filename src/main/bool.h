#ifndef ENORMEDB_BOOL_H
#define ENORMEDB_BOOL_H

#include "io.h"

typedef enum { false = 0, true } bool;

READ_STATUS read_bool(FILE* fp, bool* bl);
WRITE_STATUS write_bool(FILE* fp, bool* bl);

#endif //ENORMEDB_BOOL_H
