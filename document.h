#ifndef ENORMEDB_DOCUMENT_H
#define ENORMEDB_DOCUMENT_H

#include <inttypes.h>

struct document {
    int32_t bytes;
    element elements[];
};

#endif
