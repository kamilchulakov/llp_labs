#include "test_types.h"
#include "../main/db_file.h"

void test_uint(FILE* fp) {
    print_running_test("test_uint");
    uint32_t* val = malloc(sizeof(uint32_t));
    *val = 1000;
    fseek(fp, 0, SEEK_SET);
    assert(write_uint(fp, val) == WRITE_OK);
    uint32_t* expected = malloc(sizeof(uint32_t));
    fseek(fp, 0, SEEK_SET);
    assert(read_uint(fp, expected) == READ_OK);
    assert(*val == *expected);
}

void test_bool(FILE* fp) {
    print_running_test("test_bool");
    bool* val = malloc(sizeof(bool));
    *val = true;
    fseek(fp, 0, SEEK_SET);
    assert(write_bool(fp, val) == WRITE_OK);
    bool* expected = malloc(sizeof(bool));
    fseek(fp, 0, SEEK_SET);
    assert(read_bool(fp, expected) == READ_OK);
    assert(*val == *expected);
    free(val);
    free(expected);
}

void test_types() {
    print_running("test_types");
    db_handler* db = open_db_file("tmp");
    test_bool(db->fp);
    test_uint(db->fp);
    utilize_db_file(db);
}