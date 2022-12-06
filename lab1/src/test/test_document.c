#include "test_document.h"
#include "../main/document.h"
#include "../main/element.h"


test_status test_write_and_read(FILE* fp) {
    document* doc = create_document(1);
    doc->data = create_element_int32("8B", 64);
    open_test_file_write(fp);
    assert(write_document(fp, doc) == WRITE_OK);
    doc = malloc(sizeof(document));
    open_test_file_read(fp);
    assert(read_document(fp, doc) == READ_OK);
    element* el = (element *) doc->data;
    element* expected_el = create_element_int32("8B", 64);
    assert_element_equals(el, expected_el);
    return TEST_OK;
}
