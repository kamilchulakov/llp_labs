#include <sys/stat.h>

#include "mem.h"
#include "../main/query_executor.h"
#include "../test/test.h"
#include "../main/pager.h"

void insert_schema(db_handler* db) {
    create_schema_query query = {new_collection(string_of("mem"), new_schema(4))};
    query.col->sch->fields[0].e_type = INT32;
    query.col->sch->fields[0].e_name = string_of("int_field");
    query.col->sch->fields[1].e_type = BOOLEAN;
    query.col->sch->fields[1].e_name = string_of("bool_field");
    query.col->sch->fields[2].e_type = DOUBLE;
    query.col->sch->fields[2].e_name = string_of("double_field");
    query.col->sch->fields[3].e_type = STRING;
    query.col->sch->fields[3].e_name = string_of("string_field");
    create_schema(db, &query);
}

void insert_documents(db_handler* db, int amount) {
    insert_query query = {NULL, string_of("mem"), create_document(0)};
    for (int i = 0; i < amount; ++i) {
        query.doc = create_document(4);
        query.doc->data.elements[0] = *create_element(INT32, "int_field");
        query.doc->data.elements[0].int_data = i;
        query.doc->data.elements[1] = *create_element(BOOLEAN, "bool_field");
        query.doc->data.elements[1].bool_data = i % 2;
        query.doc->data.elements[2] = *create_element(DOUBLE, "double_field");
        query.doc->data.elements[2].double_data = ((double) i) / 3;
        query.doc->data.elements[3] = *create_element(STRING, "string_field");
        query.doc->data.elements[3].string_data = bigString();
        collection_insert(db, &query);
    }
}

void check_free_document_pages(db_handler* db, int amount) {
    uint32_t currPg = db->pagerData->firstFreeDocumentPageId;
    int count = 0;
    page* pg;
    while (currPg != -1) {
        pg = get_page(db, currPg);
        currPg = pg->prevPageId;
        if (pg->type == PAGE_DOCUMENT)
            count++;
    }
    printf("MEM: free document pages: %d\n", count);
    assert(count >= amount);
}

void check_free_string_pages(db_handler* db, int amount) {
    uint32_t currPg = db->pagerData->firstFreeStringPageId;
    int count = 0;
    page* pg;
    while (currPg != -1) {
        pg = get_page(db, currPg);
        currPg = pg->prevPageId;
        if (pg->type == PAGE_STRING)
            count++;
    }
    printf("MEM: free string pages: %d\n", count);
    assert(count >= amount);
}

void clear_all_documents(db_handler* db) {
    find_query findQuery = {string_of("mem"), NULL};
    collection_remove(db, &findQuery);
}

__off_t get_mem() {
    struct stat st;
    stat("bench-mem", &st);
    return st.st_size;
}

void bench_mem(int amount) {
    print_running("BENCHMARK MEMORY USAGE");

    db_handler* db = open_db_file("bench-mem");

    insert_schema(db);
    insert_documents(db, amount);
    printf("MEM: after insert: %ld\n", get_mem());
    check_free_document_pages(db, 0);
    check_free_string_pages(db, 0);
    assert(db->pagerData->pageIdSeq == 3*amount+2); // 2 = 1 initial + 1 schema

    clear_all_documents(db);
    printf("MEM: clear\n");
    check_free_document_pages(db, amount);
    check_free_string_pages(db, 2*amount);

    insert_documents(db, amount);
    printf("MEM: after insert: %ld\n", get_mem());
    check_free_document_pages(db, 0);
    check_free_string_pages(db, 0);
    assert(db->pagerData->pageIdSeq == 3*amount+2);

    utilize_db_file(db);
}