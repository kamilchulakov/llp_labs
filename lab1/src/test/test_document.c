#include "test_document.h"
#include "../main/db_file.h"

void test_document_element(FILE* fp) {
    print_running_test("test_document1");
    document* doc = create_document(1);
    doc->data.elements = create_element_int32("8B", 64);
    fseek(fp, 0, SEEK_SET);
    assert(write_document(fp, doc) == WRITE_OK);
    doc = malloc(sizeof(document));
    fseek(fp, 0, SEEK_SET);
    assert(read_document(fp, doc) == READ_OK);
    assert(doc->data.count == 1);
    assert(doc->data.nextPage == -1);
    assert(doc->data.elements != NULL);
    element* expected_el = create_element_int32("8B", 64);
    assert_element_equals( doc->data.elements, expected_el);
}

void test_copy_document() {
    print_running_test("test_copy_document");
    document* original = create_document(3);
    original->data.elements[0] = *create_element_int32("el-0", 10);
    original->data.elements[1] = *create_element_int32("el-1", 11);
    original->data.elements[2] = *create_element_int32("el-1", 12);
    original->parentPage = 10;
    original->childPage = 5;
    original->brotherPage = 3;
    original->prevBrotherPage = 2;
    original->collectionPage = 1;

    document* copy = copy_document(original, 0, 0, false);
    assert(copy->collectionPage == -1);
    assert(copy->prevBrotherPage == -1);
    assert(copy->brotherPage == -1);
    assert(copy->parentPage == -1);
    assert(copy->childPage == -1);
    assert(copy->data.count == 1);
    assert(copy->data.elements[0].int_data == 10);

    copy = copy_document(original, 1, 2, true);
    assert(copy->collectionPage == 1);
    assert(copy->prevBrotherPage == 2);
    assert(copy->brotherPage == 3);
    assert(copy->parentPage == 10);
    assert(copy->childPage == 5);
    assert(copy->data.count == 2);
    assert(copy->data.elements[0].e_field->e_type == INT32);
    assert(copy->data.elements[0].int_data == 11);
    assert(copy->data.elements[1].e_field->e_type == INT32);
    assert(copy->data.elements[1].int_data == 12);
}

void test_document() {
    print_running("test_document");
    db_handler* db = open_db_file("tmp");
    test_document_element(db->fp);
    test_copy_document();
    utilize_db_file(db);
}
