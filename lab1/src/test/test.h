#ifndef ENORMEDB_TEST_H
#define ENORMEDB_TEST_H

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../main/io.h"
#include "../main/types.h"
#define TESTS_COUNT (size_t) 0

typedef enum {
    TEST_OK = 0,
    TEST_ERROR
} test_status;

void open_test_file_write(FILE* fp);
void open_test_file_read(FILE* fp);

void assert_int32_equals(void* first, void* second);
void assert_element_equals(void* first, void* second);

#endif //ENORMEDB_TEST_H
