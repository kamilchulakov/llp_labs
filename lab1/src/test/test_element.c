#include "test_element.h"
#include "../main/element.h"

// should cover at least element types: 4 bytes int[x], double[x], string[x], bool[x]

test_status test_int32_element(FILE* fp) {
    print_running_test("test_int32_element");
    element* el = create_element_int32("int", INT32_MAX);
    open_test_file_write(fp);
    assert(write_element(fp, el) == WRITE_OK);
    open_test_file_read(fp);
    assert(read_element(fp, el) == READ_OK);
    assert(el->e_field->e_type == INT32);
    string expected_field_name = {.len = 3, .ch = "int"};
    assert(strcmp(el->e_field->e_name.ch, expected_field_name.ch) == 0);
    assert(el->e_field->e_name.len == expected_field_name.len);
    assert(el->int_data == INT32_MAX);
    return TEST_OK;
}

test_status test_double_element(FILE* fp) {
    print_running_test("test_double_element");
    element* el = create_element(DOUBLE, "double");
    double data = 4.2;
    el->double_data = data;
    open_test_file_write(fp);
    assert(write_element(fp, el) == WRITE_OK);
    open_test_file_read(fp);
    assert(read_element(fp, el) == READ_OK);
    assert(el->e_field->e_type == DOUBLE);
    string expected_field_name = {.len = 6, .ch = "double"};
    assert(strcmp(el->e_field->e_name.ch, expected_field_name.ch) == 0);
    assert(el->e_field->e_name.len == expected_field_name.len);
    assert(el->double_data == 4.2);
    return TEST_OK;
}

test_status test_bool_element(FILE* fp) {
    print_running_test("test_bool_element");
    element* el = create_element(BOOLEAN, "fails");
    bool bl = false;
    el->bool_data = bl;
    open_test_file_write(fp);
    assert(write_element(fp, el) == WRITE_OK);
    open_test_file_read(fp);
    assert(read_element(fp, el) == READ_OK);
    assert(el->e_field->e_type == BOOLEAN);
    string expected_field_name = {.len = 5, .ch = "fails"};
    assert(strcmp(el->e_field->e_name.ch, expected_field_name.ch) == 0);
    assert(el->e_field->e_name.len == expected_field_name.len);
    assert(el->bool_data == false);
    return TEST_OK;
}

test_status test_string_element(FILE* fp) {
    print_running_test("test_string_element");
    element* el = create_element(STRING, "str");
    el->string_data = malloc(sizeof(string));
    el->string_data->len = 5;
    el->string_data->ch = "data5";
    open_test_file_write(fp);
    assert(write_element(fp, el) == WRITE_OK);
    open_test_file_read(fp);
    el = create_element(BOOLEAN, "not bool");
    assert(read_element(fp, el) == READ_OK);
    assert(el->e_field->e_type == STRING);
    string expected_field_name = {.len = 3, .ch = "str"};
    assert(strcmp(el->e_field->e_name.ch, expected_field_name.ch) == 0);
    assert(el->e_field->e_name.len == expected_field_name.len);
    assert(el->string_data->len == 5);
    assert(strcmp(el->string_data->ch, "data5") == 0);
    return TEST_OK;
}

test_status test_element(FILE* fp) {
    print_running("test_element");
    test_bool_element(fp);
    test_int32_element(fp);
    test_double_element(fp);
    test_string_element(fp);
    return TEST_OK;
}