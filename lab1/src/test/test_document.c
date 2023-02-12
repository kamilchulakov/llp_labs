#include "test_document.h"
#include "../main/db_file.h"

test_status test_document_element(FILE* fp) {
    print_running_test("test_document1");
    document* doc = create_document(1);
    doc->data = create_element_int32("8B", 64);
    fseek(fp, 0, SEEK_SET);
    assert(write_document(fp, doc) == WRITE_OK);
    doc = malloc(sizeof(document));
    fseek(fp, 0, SEEK_SET);
    assert(read_document(fp, doc) == READ_OK);
    element* expected_el = create_element_int32("8B", 64);
    assert_element_equals( doc->data, expected_el);
    return TEST_OK;
}

test_status test_document() {
    print_running("test_document");
    db_handler* db = open_db_file("tmp");
    test_document_element(db->fp);
    utilize_db_file(db);
    return TEST_OK;
}
