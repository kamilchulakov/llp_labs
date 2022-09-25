#include <stdio.h>

#include "bson.h"
#include "document.h"

int main() {
    document* doc = create_document(0, 1);
    element el = { STRING, "Hello, World\n", NULL };
    doc->elements[0] = el;
    printf("%d %s", el.e_type, el.e_name);
    return 0;
}
