#include "test.h"
#include "test_element.h"
#include "test_document.h"

int main() {
    FILE* fp = fopen("tmp", "w");
    test_element(fp);
    return 0;
}