#include "test_element.h"
#include "../main/element.h"
#include "../main/db_file.h"

// should cover at least element types: 4 bytes int[x], double[x], string[x], bool[x]

test_status test_int32_element(FILE* fp) {
    print_running_test("test_int32_element");
    element* el = create_element_int32("int", INT32_MAX);
    seek_db_header(fp);
    assert(write_element(fp, el) == WRITE_OK);
    seek_db_header(fp);
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
    seek_db_header(fp);
    assert(write_element(fp, el) == WRITE_OK);
    seek_db_header(fp);
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
    seek_db_header(fp);
    assert(write_element(fp, el) == WRITE_OK);
    seek_db_header(fp);
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
    seek_db_header(fp);
    assert(write_element(fp, el) == WRITE_OK);
    seek_db_header(fp);
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

test_status test_element() {
    print_running("test_element");
    db_handler* db = open_db_file("tmp");
    test_bool_element(db->fp);
    test_int32_element(db->fp);
    test_double_element(db->fp);
    test_string_element(db->fp);
    utilize_db_file(db);
    return TEST_OK;
}