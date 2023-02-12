#include "test_pager.h"
#include "../main/db_file.h"
#include "../main/pager.h"

void test_allocate_and_get(db_handler* db) {
    print_running_test("test_allocate_and_get");
    assert(db->db_file_header->first_collection_page_id == -1);
    assert(db->db_file_header->first_free_collection_page_id == -1);

    allocate_page_typed(db, PAGE_COLLECTION);
    page* pg = get_page(db, 1);

    assert(pg->type == PAGE_COLLECTION);
    assert(pg->page_id == 1);
    assert(pg->next_page_id == -1);
    assert(pg->used_mem == 42); // this magic will be replaced one day
}

void test_more_allocate_and_get(db_handler* db) {
    print_running_test("test_more_allocate_and_get");
    assert(db->db_file_header->first_collection_page_id == 1);
    assert(db->db_file_header->first_free_collection_page_id == -1);

    get_free_collection_page(db);
    page* pg = get_page(db, 2);

    assert(pg->type == PAGE_COLLECTION);
    assert(pg->page_id == 2);
    assert(pg->next_page_id == 1);
    assert(pg->used_mem == 42);
    assert(db->db_file_header->first_collection_page_id == 2);

    get_free_collection_page(db);
    pg = get_page(db, 3);

    assert(pg->type == PAGE_COLLECTION);
    assert(pg->page_id == 3);
    assert(pg->next_page_id == 2);
    assert(pg->used_mem == 42);
    assert(db->db_file_header->first_collection_page_id == 3);

    pg = get_page(db, 2);

    assert(pg->type == PAGE_COLLECTION);
    assert(pg->page_id == 2);
    assert(pg->next_page_id == 1);
    assert(pg->used_mem == 42);
    assert(db->db_file_header->first_collection_page_id == 3);
}

void test_pager() {
    print_running("test_pager");
    db_handler* db = open_db_file("tmp");
    test_allocate_and_get(db);
    test_more_allocate_and_get(db);
    utilize_db_file(db);
}