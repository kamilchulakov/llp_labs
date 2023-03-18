#include "test_collection.h"

void test_collection_write_and_read(db_handler* db) {
    print_running_test("test_collection_write_and_read");
    collection* col = new_collection(string_of("bob"), new_schema(1));
    field fld = {.e_name = string_of("field"), .e_type = STRING};
    col->sch->fields[0] = fld;
    seek_db_header(db->fp);
    assert(write_collection(db->fp, col) == WRITE_OK);
    collection* col2 = new_collection(string_of("not bob"), new_schema(0));
    seek_db_header(db->fp);
    assert(read_collection(db->fp,col2) == READ_OK);
    assert(string_equals(col->name, col2->name));
}

void test_collection() {
    print_running("test_collection");
    db_handler* db = open_db_file("tmp");
    test_collection_write_and_read(db);
    utilize_db_file(db);
}