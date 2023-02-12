#include "test.h"
#include "test_element.h"
#include "test_document.h"
#include "test_collection.h"
#include "test_queries.h"
#include "test_types.h"
#include "test_pager.h"

int main() {
    test_types();
    test_element();
    test_document();
    test_collection();
    test_pager();
    test_queries();
    return 0;
}