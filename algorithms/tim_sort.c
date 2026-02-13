#include <stdlib.h>

#include "../operations.h"

#define RUN 32

static int min_value(int a, int b) {
    return (a < b) ? a : b;
}

static void insertion_sort(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int temp = arr[i];
        int j = i - 1;

        while (j >= left && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

static void merge(int arr[], int left, int mid, int right) {
    int len1 = mid - left + 1;
    int len2 = right - mid;

    int *left_arr = (int *)malloc((size_t)len1 * sizeof(int));
    int *right_arr = (int *)malloc((size_t)len2 * sizeof(int));

    if (!left_arr || !right_arr) {
        free(left_arr);
        free(right_arr);
        return;
    }

    for (int i = 0; i < len1; i++) {
        left_arr[i] = arr[left + i];
    }
    for (int i = 0; i < len2; i++) {
        right_arr[i] = arr[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < len1 && j < len2) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k++] = left_arr[i++];
        } else {
            arr[k++] = right_arr[j++];
        }
    }

    while (i < len1) {
        arr[k++] = left_arr[i++];
    }

    while (j < len2) {
        arr[k++] = right_arr[j++];
    }

    free(left_arr);
    free(right_arr);
}

void tim_sort(int arr[], int n) {
    if (n <= 1) {
        return;
    }

    for (int i = 0; i < n; i += RUN) {
        int right = min_value(i + RUN - 1, n - 1);
        insertion_sort(arr, i, right);
    }

    for (int size = RUN; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = min_value(left + size - 1, n - 1);
            int right = min_value(left + 2 * size - 1, n - 1);

            if (mid < right) {
                merge(arr, left, mid, right);
            }
        }
    }
}
