#ifndef MONGODB_DOCUMENT_H
#define MONGODB_DOCUMENT_H

#include <inttypes.h>

struct document {
    int32_t bytes;
    element elements[];
};

#endif
