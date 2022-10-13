#include "test_element.h"
#include "../main/element.h"

test_status test_bool_element(FILE* fp) {
    element* el = create_element(BOOLEAN, "fails");
    bool bl = false;
    el->e_data = &bl;
    write_element(fp, el);
    read_element(fp, el);
    assert(el->e_field.e_type == BOOLEAN);
    assert(strcmp(el->e_field.e_name, "fails") == 0);
    assert(*((bool*)el->e_data) == false);
    return TEST_OK;
}

test_status test_empty_element(FILE* fp) {
    element* el = create_element(EMPTY, "void");
    write_element(fp, el);
    read_element(fp, el);
    assert(el->e_field.e_type == EMPTY);
    assert(strcmp(el->e_field.e_name, "void") == 0);
    assert(el->e_data == NULL);
    return TEST_OK;
}

test_status test_element(FILE* fp) {
    test_bool_element(fp);
    test_empty_element(fp);
}