#ifndef ENORMEDB_QUERY_H
#define ENORMEDB_QUERY_H

#include "bson.h"
#include "document.h"
#include "schema.h"
#include "collection.h"

struct insert_in_collection_query {
    collection col;
    document* doc;
};

struct select_from_collection_query {
    collection col;
    // filter fil;
};

struct delete_collection_query {
    collection col;
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

struct add_to_document_schema_query {
    document doc;
    field* fields;
};

struct delete_from_document_schema_query {
    document doc;
    field* fields;
};

#endif //ENORMEDB_QUERY_H
