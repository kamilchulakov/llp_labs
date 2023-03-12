#include "test_pager.h"
#include "../main/db_file.h"
#include "../main/pager.h"

void test_allocate_and_get(db_handler* db) {
    print_running_test("test_allocate_and_get");
    assert(db->pagerData->pageIdSeq == 1);
    assert(db->pagerData->lastCollectionPage == -1);
    assert(db->pagerData->firstFreeCollectionPageId == -1);

    allocate_page_typed(db, PAGE_COLLECTION);
    page* pg = get_page(db, 1);

    assert(db->pagerData->pageIdSeq == 2);
    assert(pg->type == PAGE_COLLECTION);
    assert(pg->page_id == 1);
    assert(pg->prevPageId == -1);
    assert(pg->used_mem == 42); // TODO: this magic will be replaced one day
}

void test_more_allocate_and_get(db_handler* db) {
    print_running_test("test_more_allocate_and_get");
    assert(db->pagerData->lastCollectionPage == 1);
    assert(db->pagerData->firstFreeCollectionPageId == -1);

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
    assert(db->pagerData->firstFreeCollectionPageId == -1);

    assert(free_page(db, 2) == WRITE_OK);
    assert(db->pagerData->lastCollectionPage == 3);
    assert(db->pagerData->firstFreeCollectionPageId == 2);

    page* pg2 = get_page(db, 2);
    assert(pg2->type == PAGE_COLLECTION);
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

    assert(free_page(db, 3) == WRITE_OK);
    assert(db->pagerData->lastCollectionPage == 1);
    assert(db->pagerData->firstFreeCollectionPageId == 3);
}

void test_write_document_splits_strings(db_handler* db) {
    print_running_test("test_write_document_splits_strings");

    string* str = bigString();
    document* doc = create_document(1);
    doc->data.elements = create_element(STRING, "str");
    doc->data.elements->string_data = str;

    page* pg = allocate_page_typed(db, PAGE_DOCUMENT);
    assert(write_document_to_page(db, pg, doc) == WRITE_OK);
    assert(pg->page_id == 4);

    assert(get_page(db, 5)->type == PAGE_STRING);
    assert(get_page(db, 6)->type == PAGE_STRING);

    document* res = get_document(db, 4);
    str = bigString();
    assert_true(string_equals(res->data.elements->string_data, str));
    assert(res->data.nextPage == -1);
}

void test_write_too_big_document(db_handler* db) {
    print_running_test("test_write_too_big_document");
    page* pg = get_page(db, 4);
    document* doc = create_document(20);
    assert(write_document_to_page(db, pg, doc) == WRITE_ERROR);
}

void test_split_document(db_handler* db) {
    print_running_test("test_split_document");

    document* doc = create_document(20);
    doc->parentPage = 10;
    doc->childPage = 5;
    doc->prevCollectionDocument = 3;
    doc->prevBrotherPage = 2;
    doc->collectionPage = 1;
    for (int i = 0; i < 20; ++i) {
        doc->data.elements[i] = *create_element_int32("el", i);
    }

    page* pg = allocate_page_typed(db, PAGE_DOCUMENT);
    assert(pg->page_id == 7);
    assert(write_document_to_page_but_split_if_needed(db, pg, doc) == WRITE_OK);
    assert(get_document(db, 7)->data.nextPage == 8);
    assert(get_document(db, 8)->data.nextPage == -1);
}

void test_pager() {
    print_running("test_pager");
    db_handler* db = open_db_file("tmp");
    test_allocate_and_get(db);
    test_more_allocate_and_get(db);
    test_free_page(db);
    test_write_document_splits_strings(db);
    test_write_too_big_document(db);
    test_split_document(db);
    utilize_db_file(db);
}