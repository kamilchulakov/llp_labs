#include "db_file.h"
#include "pager.h"

int main() {
    db_handler* db_handler = open_db_file("db2");
    debug_mem_info(db_handler);
    insert_empty_page(db_handler);
    insert_empty_page(db_handler);
    debug_mem_info(db_handler);
    get_page_and_debug(db_handler, 0);
    get_page_and_debug(db_handler, 1);
    return 0;
}
