#include <assert.h>

int nondet_int();

int complex_condition(int a, int b)
{
    if (a > 1000 && b > 1000) {
        if (a == b + 53) {
            if ((a % 2 == 0) && (b % 2 != 0)) {
                return -1;
            }
        }
    }
    return 0;
}

int main()
{
    int x = nondet_int();
    int y = nondet_int();

    int result = complex_condition(x, y);

    assert(result != -1);

    return 0;
}