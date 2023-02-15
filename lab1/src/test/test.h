#ifndef ENORMEDB_TEST_H
#define ENORMEDB_TEST_H

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../main/io.h"
#include "../main/types.h"
#include "../main/query.h"

void assert_int32_equals(void* first, void* second);
void assert_element_equals(element* first_el, element* second_el);
void assert_field_equals(field* first, field* second);
void assert_true(bool pain);
void assert_false(bool pain);
void assert_result_type(query_result result, query_result_type type);

void print_running(char* test_name);
void print_running_test(char* test_name);

#endif //ENORMEDB_TEST_H
