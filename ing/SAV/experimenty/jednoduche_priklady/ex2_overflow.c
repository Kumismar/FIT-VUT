#include <assert.h>

int nondet_int();

int main()
{
    int l = nondet_int();
    int h = nondet_int();
    int m = (l + h) / 2;
    return 0;
}