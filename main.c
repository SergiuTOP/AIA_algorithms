#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#else
    #include <sys/resource.h>
#endif

#include "operations.h"

typedef struct {
    const char *key;
    const char *display_name;
    void (*sort_func)(int *, int);
} AlgorithmSpec;

static const AlgorithmSpec k_algorithms[] = {
    {"quick", "QuickSort", quick_sort},
    {"merge", "MergeSort", merge_sort},
    {"heap", "HeapSort", heap_sort},
    {"counting", "CountingSort", counting_sort},
    {"tim", "TimSort", tim_sort},
};

static const int k_algorithm_count = (int)(sizeof(k_algorithms) / sizeof(k_algorithms[0]));

static size_t get_peak_memory_kb(void) {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;

    typedef BOOL(WINAPI *get_process_memory_info_fn_t)(HANDLE, PPROCESS_MEMORY_COUNTERS, DWORD);
    static get_process_memory_info_fn_t p_get_process_memory_info = NULL;
    static int resolved = 0;

    if (!resolved) {
        HMODULE psapi_module = LoadLibraryA("psapi.dll");
        if (psapi_module) {
            p_get_process_memory_info =
                (get_process_memory_info_fn_t)GetProcAddress(psapi_module, "GetProcessMemoryInfo");
        }
        resolved = 1;
    }

    if (p_get_process_memory_info &&
        p_get_process_memory_info(GetCurrentProcess(), &pmc, (DWORD)sizeof(pmc))) {
        return (size_t)(pmc.PeakWorkingSetSize / 1024);
    }
    return 0;
#else
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        #ifdef __APPLE__
            return (size_t)(usage.ru_maxrss / 1024); /* bytes -> KB */
        #else
            return (size_t)usage.ru_maxrss; /* already KB on Linux */
        #endif
    }
    return 0;
#endif
}

static void print_usage(const char *program_name) {
    printf("Usage:\n");
    printf("  %s <algo> in.txt\n", program_name);
    printf("  %s <algo> in.txt stdout\n", program_name);
    printf("  %s <algo> in.txt out.txt\n", program_name);
    printf("  %s <algo1> <algo2> ... in.txt\n", program_name);
    printf("  %s all in.txt\n", program_name);
    printf("Algorithms: quick, merge, heap, counting, tim\n");
}

static const AlgorithmSpec *find_algorithm(const char *name) {
    for (int i = 0; i < k_algorithm_count; i++) {
        if (strcmp(name, k_algorithms[i].key) == 0) {
            return &k_algorithms[i];
        }
    }
    return NULL;
}

static int is_algorithm_name(const char *name) {
    return find_algorithm(name) != NULL;
}

static int *load_input_file(const char *input_file, int *out_n) {
    FILE *fp = fopen(input_file, "r");
    if (!fp) {
        return NULL;
    }

    int capacity = 1024;
    int count = 0;
    int *arr = (int *)malloc((size_t)capacity * sizeof(int));
    if (!arr) {
        fclose(fp);
        return NULL;
    }

    int value;
    while (fscanf(fp, "%d", &value) == 1) {
        if (count == capacity) {
            capacity *= 2;
            int *tmp = (int *)realloc(arr, (size_t)capacity * sizeof(int));
            if (!tmp) {
                free(arr);
                fclose(fp);
                return NULL;
            }
            arr = tmp;
        }
        arr[count++] = value;
    }
    fclose(fp);

    if (count == 0) {
        free(arr);
        return NULL;
    }

    int *tmp = (int *)realloc(arr, (size_t)count * sizeof(int));
    if (tmp) {
        arr = tmp;
    }
    *out_n = count;
    return arr;
}

static int int_compare_asc(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

static void reverse_array(int *arr, int n) {
    int i = 0;
    int j = n - 1;
    while (i < j) {
        int t = arr[i];
        arr[i] = arr[j];
        arr[j] = t;
        i++;
        j--;
    }
}

static int *clone_array(const int *arr, int n) {
    int *copy = (int *)malloc((size_t)n * sizeof(int));
    if (!copy) {
        return NULL;
    }
    memcpy(copy, arr, (size_t)n * sizeof(int));
    return copy;
}

static void write_to_stdout(const int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i + 1 < n) {
            printf(" ");
        }
    }
    printf("\n");
}

static int write_to_file(FILE *fp, const int *arr, int n) {
    if (!fp) {
        return 0;
    }
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d\n", arr[i]);
    }
    return 1;
}

int main(int argc, char *argv[]) {
    const char *input_file = NULL;
    const char *output_target = NULL;
    int input_index = 0;
    int n = 0;
    int *input_arr = NULL;
    int *asc_arr = NULL;
    int *desc_arr = NULL;
    int output_mode = 0; /* 0 = none, 1 = stdout, 2 = file */
    FILE *output_fp = NULL;
    const char *case_names[] = {"random/input_order", "ascending", "descending"};
    int *case_data[3];
    int selected[5] = {0, 0, 0, 0, 0};
    int selected_count = 0;

    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[argc - 1], "stdout") == 0) {
        output_target = argv[argc - 1];
        output_mode = 1;
        input_index = argc - 2;
    } else if (argc >= 4 && !is_algorithm_name(argv[argc - 2]) && strcmp(argv[argc - 2], "all") != 0) {
        output_target = argv[argc - 1];
        output_mode = 2;
        input_index = argc - 2;
    } else {
        input_index = argc - 1;
    }

    if (input_index <= 1) {
        print_usage(argv[0]);
        return 1;
    }

    input_file = argv[input_index];

    for (int i = 1; i < input_index; i++) {
        if (strcmp(argv[i], "all") == 0) {
            for (int j = 0; j < k_algorithm_count; j++) {
                if (!selected[j]) {
                    selected[j] = 1;
                    selected_count++;
                }
            }
            continue;
        }

        int found = 0;
        for (int j = 0; j < k_algorithm_count; j++) {
            if (strcmp(argv[i], k_algorithms[j].key) == 0) {
                if (!selected[j]) {
                    selected[j] = 1;
                    selected_count++;
                }
                found = 1;
                break;
            }
        }

        if (!found) {
            print_usage(argv[0]);
            return 1;
        }
    }

    if (selected_count == 0) {
        print_usage(argv[0]);
        return 1;
    }

    input_arr = load_input_file(input_file, &n);
    if (!input_arr) {
        printf("Failed to open or parse input file: %s\n", input_file);
        return 1;
    }

    asc_arr = clone_array(input_arr, n);
    desc_arr = clone_array(input_arr, n);
    if (!asc_arr || !desc_arr) {
        free(input_arr);
        free(asc_arr);
        free(desc_arr);
        printf("Failed to allocate memory for scenarios.\n");
        return 1;
    }
    qsort(asc_arr, (size_t)n, sizeof(int), int_compare_asc);
    qsort(desc_arr, (size_t)n, sizeof(int), int_compare_asc);
    reverse_array(desc_arr, n);

    case_data[0] = input_arr;
    case_data[1] = asc_arr;
    case_data[2] = desc_arr;

    if (output_mode == 2) {
        output_fp = fopen(output_target, "w");
        if (!output_fp) {
            free(input_arr);
            free(asc_arr);
            free(desc_arr);
            printf("Failed to open output file: %s\n", output_target);
            return 1;
        }
    }

    for (int a = 0; a < k_algorithm_count; a++) {
        if (!selected[a]) {
            continue;
        }

        const AlgorithmSpec *algo = &k_algorithms[a];
        printf("\n");
        printf("\n=============== ALGORITHM: %s ===============\n", algo->display_name);
        printf("ELEMENTS: %d\n", n);

        for (int i = 0; i < 3; i++) {
            int *arr_sort_only = clone_array(case_data[i], n);
            int *arr_sort_and_output = clone_array(case_data[i], n);
            if (!arr_sort_only || !arr_sort_and_output) {
                if (output_fp) {
                    fclose(output_fp);
                }
                free(input_arr);
                free(asc_arr);
                free(desc_arr);
                free(arr_sort_only);
                free(arr_sort_and_output);
                printf("Failed to allocate memory for sorting.\n");
                return 1;
            }

            clock_t start = clock();
            algo->sort_func(arr_sort_only, n);
            clock_t end = clock();
            double sorting_only_time = (double)(end - start) / CLOCKS_PER_SEC;
            double sorting_plus_output_time = 0.0;

            if (output_mode == 1 || output_mode == 2) {
                start = clock();
                algo->sort_func(arr_sort_and_output, n);
                if (output_mode == 1) {
                    write_to_stdout(arr_sort_and_output, n);
                } else {
                    fprintf(output_fp, "ALGORITHM: %s\n", algo->display_name);
                    fprintf(output_fp, "CASE: %s\n", case_names[i]);
                    if (!write_to_file(output_fp, arr_sort_and_output, n)) {
                        fclose(output_fp);
                        free(input_arr);
                        free(asc_arr);
                        free(desc_arr);
                        free(arr_sort_only);
                        free(arr_sort_and_output);
                        printf("Failed to write output file: %s\n", output_target);
                        return 1;
                    }
                    fprintf(output_fp, "\n");
                }
                end = clock();
                sorting_plus_output_time = (double)(end - start) / CLOCKS_PER_SEC;
            }

            printf("\n======\n");
            printf("CASE: %s\n", case_names[i]);
            if (output_mode == 0) {
                printf("1. Computation time (sorting only): %.6f s\n", sorting_only_time);
            } else if (output_mode == 1) {
                printf("1. Computation time (sorting only): %.6f s\n", sorting_only_time);
                printf("2. Computation time (sorting + console output): %.6f s\n", sorting_plus_output_time);
            } else {
                printf("1. Computation time (sorting only): %.6f s\n", sorting_only_time);
                printf("2. Computation time (sorting + file output): %.6f s\n", sorting_plus_output_time);
            }
            printf("Peak memory consumption: %zu KB\n", get_peak_memory_kb());
            printf("======\n");

            free(arr_sort_only);
            free(arr_sort_and_output);
        }
    }

    if (output_fp) {
        fclose(output_fp);
    }
    free(input_arr);
    free(asc_arr);
    free(desc_arr);
    return 0;
}
