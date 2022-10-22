#include "test.h"

void open_test_file_write(FILE* fp) {
    fclose(fp);
    fp = fopen("tmp", "w");
}

void open_test_file_read(FILE* fp) {
    fclose(fp);
    fp = fopen("tmp", "r");
}