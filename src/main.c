#include <stdio.h>

#include "bson.h"
#include "document.h"
#include "db_file.h"

int main() {
    create_db_file("../db");
    printf("%d", read_mem_info(fopen("../db", "r")).allocated_mem);
    return 0;
}
