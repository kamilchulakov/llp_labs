#include "test.h"
#include "../main/element.h"

void open_test_file_write(FILE* fp) {
    fclose(fp);
    fp = fopen("tmp", "w");
}

void open_test_file_read(FILE* fp) {
    fclose(fp);
    fp = fopen("tmp", "r");
}

void assert_int32_equals(void* first, void* second) {
    assert(*(int32_t *) first == *(int32_t *) second);
}

void assert_string_equals(void* first, void* second) {
    assert(string_equals(first, second));
}

void assert_boolean_equals(void* first, void* second) {
    assert(*(bool*) first == *(bool*) second );
}

void assert_element_equals(void* first, void* second) {
    element* first_el = (element *) first;
    element* second_el = (element *) second;
    ELEMENT_TYPE type = first_el->e_field.e_type;

    assert(type == second_el->e_field.e_type);
    assert_string_equals(&first_el->e_field.e_name, &second_el->e_field.e_name);

    if (type != EMPTY) {
        if (type == INT32)
            assert_int32_equals(first_el->e_data, second_el->e_data);
        else if (type == STRING)
            assert_string_equals(first_el->e_data, second_el->e_data);
        else if (type == BOOLEAN)
            assert_boolean_equals(first_el->e_data, second_el->e_data);
    }
}