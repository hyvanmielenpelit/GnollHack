#include <stdio.h>
#include <stdint.h>
#define OBJFLAG 0x00000200UL
int main() { 
    uint64_t flags = 0xFFFFFFFFFFFFFFFFULL; 
    flags &= ~OBJFLAG; 
    printf("%llx\n", (unsigned long long)flags); 
    return 0; 
}
