#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "operations.h"

// Handle directory creation for Windows/Linux
#ifdef _WIN32
    #include <direct.h>
    #define make_dir(path) _mkdir(path)
#else
    #include <sys/stat.h>
    #define make_dir(path) mkdir(path, 0777)
#endif

// 1. Generate the 3 required input files ONCE
void generate_input_files(int n) {
    remove("random.txt");
    remove("ascending.txt");
    remove("descending.txt");

    FILE *fRand = fopen("random.txt", "w");
    FILE *fAsc = fopen("ascending.txt", "w");
    FILE *fDesc = fopen("descending.txt", "w");

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        fprintf(fRand, "%d\n", rand() % 1000000);
        fprintf(fAsc, "%d\n", i + 1);
        fprintf(fDesc, "%d\n", n - i);
    }

    fclose(fRand); fclose(fAsc); fclose(fDesc);
    printf("[System] Input files (random, ascending, descending) created for N=%d\n", n);
}

int* load_data(const char* filename, int n) {
    int *arr = (int*)malloc(n * sizeof(int));
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;
    for (int i = 0; i < n; i++) fscanf(fp, "%d", &arr[i]);
    fclose(fp);
    return arr;
}

void run_benchmark(const char* algo_name, void (*sort_func)(int*, int), int n, const char* input_file) {
    // SCENARIO 1: Sorting Only
    int *arr1 = load_data(input_file, n);
    clock_t s1 = clock();
    sort_func(arr1, n);
    clock_t e1 = clock();
    double time_only = (double)(e1 - s1) / CLOCKS_PER_SEC;
    free(arr1);

    // SCENARIO 2: Sorting + Console Output
    int *arr2 = load_data(input_file, n);
    clock_t s2 = clock();
    sort_func(arr2, n);
    for (int i = 0; i < n; i++) printf("%d ", arr2[i]);
    printf("\n");
    clock_t e2 = clock();
    double time_console = (double)(e2 - s2) / CLOCKS_PER_SEC;
    free(arr2);

    // SCENARIO 3: Sorting + File Output (Creates ONE file per algorithm)
    make_dir("sorted_output_files");
    char out_path[256];
    sprintf(out_path, "sorted_output_files/%s_sorted.txt", algo_name);

    int *arr3 = load_data(input_file, n);
    clock_t s3 = clock();
    sort_func(arr3, n);
    FILE *fOut = fopen(out_path, "w");
    if (fOut) {
        for (int i = 0; i < n; i++) fprintf(fOut, "%d\n", arr3[i]);
        fclose(fOut);
    }
    clock_t e3 = clock();
    double time_file = (double)(e3 - s3) / CLOCKS_PER_SEC;
    free(arr3);

    printf("\n==========================================================\n");
    printf("ALGORITHM: %s | INPUT: %s\n", algo_name, input_file);
    printf("1. Sorting time only:          %.6f s\n", time_only);
    printf("2. Sorting + console output:   %.6f s\n", time_console);
    printf("3. Sorting + file output:      %.6f s\n", time_file);
    printf("==========================================================\n");
}

int main() {
    int n;
    printf("Enter number of elements (100, 1000, 10000, 100000, 1000000): ");
    scanf("%d", &n);

    // Generate files once for the whole run
    generate_input_files(n);

    const char* cases[] = {"random.txt", "ascending.txt", "descending.txt"};

    for (int i = 0; i < 3; i++) {
        printf("\n>>> STARTING TESTS FOR %s <<<\n", cases[i]);
        run_benchmark("QuickSort", quick_sort, n, cases[i]);
        run_benchmark("MergeSort", merge_sort, n, cases[i]);
        run_benchmark("HeapSort", heap_sort, n, cases[i]);
        run_benchmark("CountingSort", counting_sort, n, cases[i]);
        run_benchmark("TimSort", tim_sort, n, cases[i]);
    }

    return 0;
}