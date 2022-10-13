#ifndef ENORMEDB_IO_H
#define ENORMEDB_IO_H

#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif

typedef enum {
    READ_OK = 0,
    READ_ERROR
} READ_STATUS;

typedef enum {
    WRITE_OK = 0,
    WRITE_ERROR
} WRITE_STATUS;

#endif //ENORMEDB_IO_H
