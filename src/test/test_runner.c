#include "test.h"
#include "test_element.h"

int main() {
    FILE* fp = fopen("tmp", "w");
    test_element(fp);
    return 0;
}