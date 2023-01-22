#include "test.h"
#include "test_element.h"
#include "test_document.h"

int main() {
    FILE* fp = fopen("tmp", "w");
    test_element(fp);
    fp = fopen("tmp", "w");
    test_document(fp);
    return 0;
}