#include "db_file.h"

int main() {
    db_handler* db = open_db_file("db2");

    utilize_db_file(db);
    return 0;
}
