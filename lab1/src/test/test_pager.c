#include "test_pager.h"
#include "../main/db_file.h"
#include "../main/pager.h"

void test_allocate_and_get(db_handler* db) {
    print_running_test("test_allocate_and_get");
    assert(db->pagerData->lastCollectionPage == -1);
    assert(db->pagerData->first_free_collection_page_id == -1);

    allocate_page_typed(db, PAGE_COLLECTION);
    page* pg = get_page(db, 1);

    assert(pg->type == PAGE_COLLECTION);
    assert(pg->page_id == 1);
    assert(pg->prevPageId == -1);
    assert(pg->used_mem == 42); // TODO: this magic will be replaced one day
}

void test_more_allocate_and_get(db_handler* db) {
    print_running_test("test_more_allocate_and_get");
    assert(db->pagerData->lastCollectionPage == 1);
    assert(db->pagerData->first_free_collection_page_id == -1);

    get_free_collection_page(db);
    page* pg = get_page(db, 2);

    assert(pg->type == PAGE_COLLECTION);
    assert(pg->page_id == 2);
    assert(pg->prevPageId == 1);
    assert(pg->nextPageId == -1);
    assert(pg->used_mem == 42);
    assert(db->pagerData->lastCollectionPage == 2);

    get_free_collection_page(db);
    pg = get_page(db, 3);

    assert(pg->type == PAGE_COLLECTION);
    assert(pg->page_id == 3);
    assert(pg->prevPageId == 2);
    assert(pg->nextPageId == -1);
    assert(pg->used_mem == 42);
    assert(db->pagerData->lastCollectionPage == 3);

    pg = get_page(db, 2);

    assert(pg->type == PAGE_COLLECTION);
    assert(pg->page_id == 2);
    assert(pg->prevPageId == 1);
    assert(pg->nextPageId == 3);
    assert(pg->used_mem == 42);
    assert(db->pagerData->lastCollectionPage == 3);
}

void test_free_page(db_handler* db) {
    print_running_test("test_free_page");
    assert(db->pagerData->lastCollectionPage == 3);
    assert(db->pagerData->first_free_collection_page_id == -1);

    assert(free_page(db, 2) == WRITE_OK);
    assert(db->pagerData->lastCollectionPage == 3);
    assert(db->pagerData->first_free_collection_page_id == 2);

    page* pg2 = get_page(db, 2);
    assert(pg2->type == PAGE_EMPTY);
    assert(pg2->page_id == 2);
    assert(pg2->nextPageId == -1);
    assert(pg2->prevPageId == -1);
    assert(pg2->used_mem == 42);

    page* pg3 = get_page(db, 3);
    assert(pg3->type == PAGE_COLLECTION);
    assert(pg3->page_id == 3);
    assert(pg3->nextPageId == -1);
    assert(pg3->prevPageId == 1);
    assert(pg3->used_mem == 42);

    page* pg1 = get_page(db, 1);
    assert(pg1->type == PAGE_COLLECTION);
    assert(pg1->page_id == 1);
    assert(pg1->nextPageId == 3);
    assert(pg1->prevPageId == -1);
    assert(pg1->used_mem == 42);
}

void test_pager() {
    print_running("test_pager");
    db_handler* db = open_db_file("tmp");
    test_allocate_and_get(db);
    test_more_allocate_and_get(db);
    test_free_page(db);
    utilize_db_file(db);
}