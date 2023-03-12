#include "test.h"
#include "../main/element.h"

void assert_int32_equals(void* first, void* second) {
    assert(*(int32_t *) first == *(int32_t *) second);
}

void assert_double_equals(void* first, void* second) {
    assert(*(double *) first == *(double *) second);
}

void assert_true(bool pain) {
    assert(pain == true);
}

void assert_false(bool pain) {
    assert(pain == false);
}

void assert_result_type(query_result result, query_result_type type) {
    assert(result.type == type);
}

void assert_string_equals(string* first, string* second) {
    assert(string_equals(first, second));
}

void assert_boolean_equals(void* first, void* second) {
    assert(*(bool*) first == *(bool*) second );
}

void assert_field_equals(field* first, field* second) {
    assert(first->e_type == second->e_type);
    assert_string_equals(first->e_name, second->e_name);
}

void assert_element_equals(element* first_el, element* second_el) {
    assert_field_equals(first_el->e_field, second_el->e_field);
    ELEMENT_TYPE type = first_el->e_field->e_type;

   if (type == INT32)
       assert_int32_equals(&first_el->int_data, &second_el->int_data);
   else if (type == STRING)
       assert_string_equals(first_el->string_data, second_el->string_data);
   else if (type == BOOLEAN)
       assert_boolean_equals(&first_el->bool_data, &second_el->bool_data);
   else if (type == DOUBLE)
       assert_double_equals(&first_el->double_data, &second_el->double_data);
}

string* bigString() {
    return string_of(
            "Зовусь я Ион Йонсен,\n"
            "Мой дом — штат Висконсин,\n"
            "В лесу я работаю тут.\n"
            "Кого ни встречаю,\n"
            "Я всем отвечаю,\n"
            "Кто спросит:\n"
            "«А как вас зовут?»\n"
            "Зовусь я Ион Йонсен,\n"
            "Мой дом — штат Висконсин,\n"
            "В лесу я работаю тут...\n"
    );
}

void print_running(char* test_name) {
    printf("\n| IN: %s\n", test_name);
}

void print_running_test(char* test_name) {
    printf("-> RUNNING: %s\n", test_name);
}