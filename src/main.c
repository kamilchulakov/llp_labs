#include "db_file.h"
#include "pager.h"

int main() {
    db_handler* db_handler = open_db_file("db2");
    debug_mem_info(db_handler);
    insert_empty_page(db_handler);
    insert_empty_page(db_handler);
    debug_mem_info(db_handler);
    return 0;
}
