int nondet_int();

int main()
{
    int arr[5];

    for (int i = 0; i < 5; i++) {
        arr[i] = nondet_int();
    }

    int n = nondet_int();
    if (n > 0) {
        for (int i = 0; i < n; i++) {
            arr[i] = 0;
        }
    }

    return 0;
}