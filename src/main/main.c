#include "db_file.h"
#include "pager.h"
#include "api.h"

int main() {
    db_handler* db_handler = open_db_file("db2");
    debug_mem_info(db_handler);
    insert_empty_page(db_handler);
    insert_empty_page(db_handler);
    insert_empty_page(db_handler);
    debug_mem_info(db_handler);
    get_page_and_debug(db_handler, 0);
    get_page_and_debug(db_handler, 1);
    get_page_and_debug(db_handler, 2);
    create_collection_in_page(db_handler, 0, "coolname");
    create_collection_in_page(db_handler, 1, "second");
    create_collection_in_page(db_handler, 2, "third");
//    debug_document(db_handler, 1);
//    update_collection_doc_id(db_handler, 0, 1);
    select_collection(db_handler, NULL);
    return 0;
}
