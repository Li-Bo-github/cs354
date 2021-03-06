// a few allocations in multiples of 4 bytes followed by frees
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
   assert(myInit(4096) == 0);
   void* ptr[4];

   ptr[0] = myAlloc(4);
   ptr[1] = myAlloc(8);
   dispMem();
}
