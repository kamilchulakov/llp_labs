#include "db_file.h"

int main() {
    db_handler* db = open_db_file("db2");

    document* doc = create_document(1);
    doc->data[0] = *create_element(STRING, "element");
    doc->data[0].string_data = string_of("HORRIBLY BIG STRING AAAAAAAAAAAAAAA");
    printf("document: %lu\n", sizeof(document));
    printf("real: %lu\n", sizeof(*doc)+sizeof(*doc->data)+sizeof(*doc->data->string_data));
    printf("calc: %lu\n", sizeof(element)*doc->elements+sizeof(uint32_t)*7);
    utilize_db_file(db);
    return 0;
}
