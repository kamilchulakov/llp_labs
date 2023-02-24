#include "test_element.h"
#include "../main/db_file.h"

// should cover at least element types: 4 bytes int[x], double[x], string[x], bool[x]

void test_int32_element(FILE* fp) {
    print_running_test("test_int32_element");
    element* el = create_element_int32("int", INT32_MAX);
    seek_db_header(fp);
    assert(write_element(fp, el) == WRITE_OK);
    seek_db_header(fp);
    assert(read_element(fp, el) == READ_OK);
    assert(el->e_field->e_type == INT32);
    string expected_field_name = {.len = 3, .ch = "int"};
    assert(strcmp(el->e_field->e_name->ch, expected_field_name.ch) == 0);
    assert(el->e_field->e_name->len == expected_field_name.len);
    assert(el->int_data == INT32_MAX);
}

void test_double_element(FILE* fp) {
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
    assert(strcmp(el->e_field->e_name->ch, expected_field_name.ch) == 0);
    assert(el->e_field->e_name->len == expected_field_name.len);
    assert(el->double_data == 4.2);
}

void test_bool_element(FILE* fp) {
    print_running_test("test_bool_element");
    element* el = create_element(BOOLEAN, "fails");
    bool bl = true;
    el->bool_data = bl;
    fseek(fp, 0, SEEK_SET);
    assert(write_element(fp, el) == WRITE_OK);
    fseek(fp, 0, SEEK_SET);
    assert(read_element(fp, el) == READ_OK);
    assert(el->e_field->e_type == BOOLEAN);
    string expected_field_name = {.len = 5, .ch = "fails"};
    assert(strcmp(el->e_field->e_name->ch, expected_field_name.ch) == 0);
    assert(el->e_field->e_name->len == expected_field_name.len);
    assert(el->bool_data == true);
}

void test_element() {
    print_running("test_element");
    db_handler* db = open_db_file("tmp");
    test_bool_element(db->fp);
    test_int32_element(db->fp);
    test_double_element(db->fp);
    utilize_db_file(db);
}