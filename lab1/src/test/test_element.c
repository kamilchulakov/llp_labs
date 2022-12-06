#include "test_element.h"
#include "../main/element.h"

// should cover at least element types: 4 bytes int[x], double[x], string[x], bool[x]

test_status test_int32_element(FILE* fp) {
    element* el = create_element_int32("int", INT32_MAX);
    open_test_file_write(fp);
    assert(write_element(fp, el) == WRITE_OK);
    open_test_file_read(fp);
    assert(read_element(fp, el) == READ_OK);
    assert(el->e_field.e_type == INT32);
    string expected_field_name = {.len = 3, .ch = "int"};
    assert(strcmp(el->e_field.e_name.ch, expected_field_name.ch) == 0);
    assert(el->e_field.e_name.len == expected_field_name.len);
    assert(*((int32_t*)el->e_data) == INT32_MAX);
    return TEST_OK;
}

test_status test_double_element(FILE* fp) {
    element* el = create_element(DOUBLE, "double");
    double data = 5.0;
    el->e_data = &data;
    open_test_file_write(fp);
    assert(write_element(fp, el) == WRITE_OK);
    open_test_file_read(fp);
    assert(read_element(fp, el) == READ_OK);
    assert(el->e_field.e_type == DOUBLE);
    string expected_field_name = {.len = 6, .ch = "double"};
    assert(strcmp(el->e_field.e_name.ch, expected_field_name.ch) == 0);
    assert(el->e_field.e_name.len == expected_field_name.len);
    assert(*((double*)el->e_data) == 5.0);
    return TEST_OK;
}

test_status test_bool_element(FILE* fp) {
    element* el = create_element(BOOLEAN, "fails");
    bool bl = false;
    el->e_data = &bl;
    open_test_file_write(fp);
    assert(write_element(fp, el) == WRITE_OK);
    open_test_file_read(fp);
    assert(read_element(fp, el) == READ_OK);
    assert(el->e_field.e_type == BOOLEAN);
    string expected_field_name = {.len = 5, .ch = "fails"};
    assert(strcmp(el->e_field.e_name.ch, expected_field_name.ch) == 0);
    assert(el->e_field.e_name.len == expected_field_name.len);
    assert(*((bool*)el->e_data) == false);
    return TEST_OK;
}

test_status test_empty_element(FILE* fp) {
    element* el = create_element(EMPTY, "void");
    open_test_file_write(fp);
    assert(write_element(fp, el) == WRITE_OK);
    open_test_file_read(fp);
    el = malloc(sizeof(element));
    assert(read_element(fp, el) == READ_OK);
    assert(el->e_field.e_type == EMPTY);
    string expected_field_name = {.len = 4, .ch = "void"};
    assert(strcmp(el->e_field.e_name.ch, expected_field_name.ch) == 0);
    assert(el->e_field.e_name.len == expected_field_name.len);
    assert(el->e_data == NULL);
    return TEST_OK;
}

test_status test_string_element(FILE* fp) {
    element* el = create_element(STRING, "str");
    el->e_data = malloc(sizeof(string));
    ((string* )el->e_data)->len = 5;
    ((string* )el->e_data)->ch = "data5";
    open_test_file_write(fp);
    assert(write_element(fp, el) == WRITE_OK);
    open_test_file_read(fp);
    el = malloc(sizeof(element));
    assert(read_element(fp, el) == READ_OK);
    assert(el->e_field.e_type == STRING);
    string expected_field_name = {.len = 3, .ch = "str"};
    assert(strcmp(el->e_field.e_name.ch, expected_field_name.ch) == 0);
    assert(el->e_field.e_name.len == expected_field_name.len);
    assert(((string* ) el->e_data)->len == 5);
    assert(strcmp(((string* ) el->e_data)->ch, "data5") == 0);
    return TEST_OK;
}

test_status test_element(FILE* fp) {
    test_empty_element(fp);
    test_bool_element(fp);
    test_int32_element(fp);
    test_string_element(fp);
    test_double_element(fp);
    return TEST_OK;
}