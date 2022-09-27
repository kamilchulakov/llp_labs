#ifndef ENORMEDB_QUERY_H
#define ENORMEDB_QUERY_H

#include "bson.h"
#include "document.h"

struct insert_in_collection_query {
    // collection col;
    document* doc;
};

struct select_from_collection_query {
    // collection col;
    // filter fil;
};

struct delete_collection_query {
    // collection col;
};

struct insert_in_document_query {
    document doc;
    element* elements;
};

struct update_document_query {
    document doc;
    element* elements;
};

struct delete_document_query {
    document doc;
};

#endif //ENORMEDB_QUERY_H
