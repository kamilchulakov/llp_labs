#include "test.h"
#include "test_element.h"
#include "test_document.h"
#include "test_collection.h"
#include "test_queries.h"

int main() {
    test_element();
    test_collection();
    test_document();
    test_queries();
    return 0;
}