#ifndef ENORMEDB_COLLECTION_H
#define ENORMEDB_COLLECTION_H

#include "document.h"
#define MAX_COLLECTION_NAME 64

typedef struct collection collection;

struct collection {
    char name[MAX_COLLECTION_NAME];
    document* doc;
};

#endif //ENORMEDB_COLLECTION_H
