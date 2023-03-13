#include <sys/stat.h>

#include "mem.h"
#include "../test/test.h"
#include "../main/pager.h"
#include "util.h"

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

__off_t get_mem() {
    struct stat st;
    stat("bench-mem", &st);
    return st.st_size;
}

void bench_mem(int loops, int amount) {
    print_running("BENCHMARK MEMORY USAGE");

    db_handler* db = open_db_file("bench-mem");

    insert_schema(db);
    off_t* memes = malloc(2 * loops * sizeof(off_t));

    for (int i = 0; i < loops; ++i) {
        printf("LOOP #%d\n", i+1);
        insert_documents(db, amount);
        memes[2 * i] = get_mem();
        printf("MEM: after insert: %ld\n", get_mem());
        check_free_document_pages(db, 0);
        check_free_string_pages(db, 0);
        assert(db->pagerData->pageIdSeq == (i+1)*3*amount+2); // 2 = 1 initial + 1 schema

        clear_all_documents(db);
        printf("MEM: clear\n");
        check_free_document_pages(db, (i+1)*amount);
        check_free_string_pages(db, (i+1)*2*amount);

        insert_documents(db, (i+1)*amount);
        memes[2 * i + 1] = get_mem();
        printf("MEM: after insert: %ld\n", get_mem());
        check_free_document_pages(db, 0);
        check_free_string_pages(db, 0);
        assert(db->pagerData->pageIdSeq == (i+1)*3*amount+2);
        printf("__________________________\n");
    }

    printf("RESULT:\n");
    for (int i = 0; i < loops; ++i) {
        printf("%d,%ld,%ld\n", i+1, memes[2 * i], memes[2 * i + 1]);
    }
    printf("__________________________\n");

    utilize_db_file(db);
}