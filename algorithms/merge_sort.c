#include <stdlib.h>

#include "../operations.h"

static void merge(int arr[], int left, int mid, int right) {
    int left_size = mid - left + 1;
    int right_size = right - mid;

    int *left_arr = (int *)malloc((size_t)left_size * sizeof(int));
    int *right_arr = (int *)malloc((size_t)right_size * sizeof(int));

    if (!left_arr || !right_arr) {
        free(left_arr);
        free(right_arr);
        return;
    }

    for (int i = 0; i < left_size; i++) {
        left_arr[i] = arr[left + i];
    }
    for (int i = 0; i < right_size; i++) {
        right_arr[i] = arr[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < left_size && j < right_size) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k++] = left_arr[i++];
        } else {
            arr[k++] = right_arr[j++];
        }
    }

    while (i < left_size) {
        arr[k++] = left_arr[i++];
    }

    while (j < right_size) {
        arr[k++] = right_arr[j++];
    }

    free(left_arr);
    free(right_arr);
}

static void merge_sort_recursive(int arr[], int left, int right) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;
    merge_sort_recursive(arr, left, mid);
    merge_sort_recursive(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void merge_sort(int arr[], int n) {
    if (n <= 1) {
        return;
    }
    merge_sort_recursive(arr, 0, n - 1);
}
