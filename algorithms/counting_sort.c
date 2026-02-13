#include <stdlib.h>

#include "../operations.h"

void counting_sort(int arr[], int n) {
    if (n <= 1) {
        return;
    }

    int min = arr[0];
    int max = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    int range = max - min + 1;
    int *count = (int *)calloc((size_t)range, sizeof(int));
    int *output = (int *)malloc((size_t)n * sizeof(int));

    if (!count || !output) {
        free(count);
        free(output);
        return;
    }

    for (int i = 0; i < n; i++) {
        count[arr[i] - min]++;
    }

    for (int i = 1; i < range; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int idx = arr[i] - min;
        output[count[idx] - 1] = arr[i];
        count[idx]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    free(count);
    free(output);
}
