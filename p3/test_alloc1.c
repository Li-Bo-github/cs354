// a simple 8 byte allocation
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"
#include <stdio.h>

int main() {
    assert(myInit(4096) == 0);
    void* ptr = myAlloc(8);
    dispMem();
    printf("%p\n",ptr);
    assert(ptr != NULL);
    exit(0);
}
