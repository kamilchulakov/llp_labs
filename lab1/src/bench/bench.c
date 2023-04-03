#include "mem.h"
#include "speed.h"

int main() {
    bench_mem(20, 5000);
    bench_speed(20, 5000);
    return 0;
}