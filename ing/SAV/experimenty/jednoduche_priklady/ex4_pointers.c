#include <stdlib.h>

int nondet_int();

#define FLAG_INIT 0x1
#define FLAG_PROCESS 0x2

int main()
{
    int flags = nondet_int();
    int* ptr = NULL;
    int buffer = 0;

    if (flags & FLAG_INIT) {
        ptr = &buffer;
    }

    if (flags & FLAG_PROCESS) {
        *ptr = 100;
    }

    return 0;
}