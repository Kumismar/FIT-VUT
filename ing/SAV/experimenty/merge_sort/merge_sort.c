#include <assert.h>

#ifndef SIZE
#    define SIZE 5
#endif

int nondet_int();

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
#ifdef BUG_CONDITION
        // Swapping sorting order
        if (L[i] > R[j])
#else
        if (L[i] <= R[j])
#endif
        {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

#ifndef BUG_MISSING_REST_COPY
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
#endif
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;  // Bezpecne

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main()
{
    int arr[SIZE];
    for (int i = 0; i < SIZE; i++) {
        arr[i] = nondet_int();
    }

    mergeSort(arr, 0, SIZE - 1);

    for (int i = 0; i < SIZE - 1; i++) {
        assert(arr[i] <= arr[i + 1]);
    }
    return 0;
}