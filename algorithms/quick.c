#include "../operations.h"

static inline void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static int median_of_three_index(int *arr, int low, int high) {
    int mid = low + (high - low) / 2;
    int a = arr[low];
    int b = arr[mid];
    int c = arr[high];

    if ((a <= b && b <= c) || (c <= b && b <= a)) {
        return mid;
    }
    if ((b <= a && a <= c) || (c <= a && a <= b)) {
        return low;
    }
    return high;
}

static int partition(int *arr, int low, int high) {
    int pivot_index = median_of_three_index(arr, low, high);
    swap(&arr[pivot_index], &arr[high]);

    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

static void quick_sort_recursive(int *arr, int low, int high) {
    while (low < high) {
        int pi = partition(arr, low, high);

        if (pi - low < high - pi) {
            quick_sort_recursive(arr, low, pi - 1);
            low = pi + 1;
        } else {
            quick_sort_recursive(arr, pi + 1, high);
            high = pi - 1;
        }
    }
}

void quick_sort(int arr[], int n) {
    if (n <= 1) {
        return;
    }
    quick_sort_recursive(arr, 0, n - 1);
}
