#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "operations.h"

typedef void (*sort_fn_t)(int[], int);

typedef struct {
    const char *name;
    sort_fn_t fn;
} algorithm_t;

typedef struct {
    int *data;
    int n;
} int_array_t;

typedef enum {
    CASE_RANDOM = 0,
    CASE_ASCENDING = 1,
    CASE_DESCENDING = 2
} input_case_t;

typedef struct {
    input_case_t input_case;
    const char *name;
    const char *path;
} input_file_case_t;

static const algorithm_t ALGORITHMS[] = {
    {"quick_sort", quick_sort},
    {"merge_sort", merge_sort},
    {"counting_sort", counting_sort},
    {"heap_sort", heap_sort},
    {"tim_sort", tim_sort},
};

static const int TEST_SIZES[] = {100, 1000, 10000, 100000, 1000000};
static const input_file_case_t INPUT_FILES[] = {
    {CASE_RANDOM, "random", "random.txt"},
    {CASE_ASCENDING, "ascending", "ascending.txt"},
    {CASE_DESCENDING, "descending", "descending.txt"},
};

static int_array_t read_input_file(const char *path) {
    FILE *in = fopen(path, "r");
    if (!in) {
        perror("Input file error");
        return (int_array_t){NULL, 0};
    }

    int capacity = 1024;
    int *arr = (int *)malloc((size_t)capacity * sizeof(int));
    if (!arr) {
        fclose(in);
        return (int_array_t){NULL, 0};
    }

    int n = 0;
    int value = 0;
    while (fscanf(in, "%d", &value) == 1) {
        if (n == capacity) {
            capacity *= 2;
            int *next = (int *)realloc(arr, (size_t)capacity * sizeof(int));
            if (!next) {
                free(arr);
                fclose(in);
                return (int_array_t){NULL, 0};
            }
            arr = next;
        }
        arr[n++] = value;
    }

    fclose(in);

    if (n == 0) {
        free(arr);
        return (int_array_t){NULL, 0};
    }

    int *fit = (int *)realloc(arr, (size_t)n * sizeof(int));
    if (fit) {
        arr = fit;
    }

    return (int_array_t){arr, n};
}

static int is_sorted_non_decreasing(const int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) {
            return 0;
        }
    }
    return 1;
}

static double seconds_between(clock_t start, clock_t end) {
    return (double)(end - start) / CLOCKS_PER_SEC;
}

static double monotonic_seconds_now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}

static int write_output(const char *destination, const int arr[], int n) {
    FILE *out = NULL;

    if (strcmp(destination, "stdout") == 0) {
        out = stdout;
    } else {
        out = fopen(destination, "w");
        if (!out) {
            perror("Output file error");
            return 1;
        }
    }

    for (int i = 0; i < n; i++) {
        fprintf(out, "%d\n", arr[i]);
    }

    if (out != stdout) {
        fclose(out);
    }

    return 0;
}

static int run_competition_mode(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s input_file [stdout|output_file]\n", argv[0]);
        return 1;
    }

    int_array_t input = read_input_file(argv[1]);
    if (!input.data) {
        fprintf(stderr, "No valid integers read from input file\n");
        return 1;
    }

    const algorithm_t selected = {"tim_sort", tim_sort};

    clock_t sort_start = clock();
    selected.fn(input.data, input.n);
    clock_t sort_end = clock();

    if (!is_sorted_non_decreasing(input.data, input.n)) {
        fprintf(stderr, "Sorting failed: output is not sorted\n");
        free(input.data);
        return 1;
    }

    double sorting_time = seconds_between(sort_start, sort_end);
    double total_time = sorting_time;

    if (argc >= 3) {
        if (write_output(argv[2], input.data, input.n) != 0) {
            free(input.data);
            return 1;
        }
        clock_t total_end = clock();
        total_time = seconds_between(sort_start, total_end);
    }

    printf("Algorithm: %s\n", selected.name);
    printf("Elements sorted: %d\n", input.n);
    printf("Sorting time only: %.6f seconds\n", sorting_time);
    printf("Sorting + output time: %.6f seconds\n", total_time);

    free(input.data);
    return 0;
}

static size_t estimate_aux_memory_bytes(const char *algo_name, int n, int min_value, int max_value) {
    if (strcmp(algo_name, "merge_sort") == 0) {
        return (size_t)n * sizeof(int);
    }

    if (strcmp(algo_name, "tim_sort") == 0) {
        return (size_t)n * sizeof(int);
    }

    if (strcmp(algo_name, "counting_sort") == 0) {
        size_t range = (size_t)(max_value - min_value + 1);
        return ((size_t)n + range) * sizeof(int);
    }

    if (strcmp(algo_name, "quick_sort") == 0) {
        size_t depth = 0;
        int x = n;
        while (x > 1) {
            x /= 2;
            depth++;
        }
        return depth * 64;
    }

    return (size_t)128;
}

static int write_case_file(const char *path, input_case_t input_case, int n) {
    remove(path);

    FILE *out = fopen(path, "w");
    if (!out) {
        perror("Dataset file error");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        int value = 0;
        if (input_case == CASE_ASCENDING) {
            value = i + 1;
        } else if (input_case == CASE_DESCENDING) {
            value = n - i;
        } else {
            value = rand() % n + 1;
        }
        fprintf(out, "%d\n", value);
    }

    fclose(out);
    return 0;
}

static int create_input_files_for_size(int n) {
    for (size_t i = 0; i < sizeof(INPUT_FILES) / sizeof(INPUT_FILES[0]); i++) {
        if (write_case_file(INPUT_FILES[i].path, INPUT_FILES[i].input_case, n) != 0) {
            return 1;
        }
    }
    return 0;
}

static void min_max(const int arr[], int n, int *out_min, int *out_max) {
    int min_value = arr[0];
    int max_value = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] < min_value) {
            min_value = arr[i];
        }
        if (arr[i] > max_value) {
            max_value = arr[i];
        }
    }

    *out_min = min_value;
    *out_max = max_value;
}

static int run_benchmark_mode(const char *csv_path, int selected_size) {
    FILE *csv = fopen(csv_path, "w");
    if (!csv) {
        perror("Benchmark output file error");
        return 1;
    }

    fprintf(csv, "algorithm,input_case,size,execution_time_seconds,estimated_aux_memory_bytes\n");
    printf("Benchmark started. Results -> %s\n", csv_path);

    const int *sizes = TEST_SIZES;
    size_t sizes_count = sizeof(TEST_SIZES) / sizeof(TEST_SIZES[0]);
    if (selected_size > 0) {
        sizes = &selected_size;
        sizes_count = 1;
    }

    for (size_t size_idx = 0; size_idx < sizes_count; size_idx++) {
        int n = sizes[size_idx];
        if (create_input_files_for_size(n) != 0) {
            fclose(csv);
            return 1;
        }

        for (size_t c = 0; c < sizeof(INPUT_FILES) / sizeof(INPUT_FILES[0]); c++) {
            for (size_t a = 0; a < sizeof(ALGORITHMS) / sizeof(ALGORITHMS[0]); a++) {
                int_array_t work = read_input_file(INPUT_FILES[c].path);
                if (!work.data || work.n != n) {
                    fprintf(stderr, "Read error for %s (expected %d values)\n", INPUT_FILES[c].path, n);
                    free(work.data);
                    fclose(csv);
                    return 1;
                }

                int min_value = 0;
                int max_value = 0;
                min_max(work.data, work.n, &min_value, &max_value);

                double start = monotonic_seconds_now();
                ALGORITHMS[a].fn(work.data, work.n);
                double end = monotonic_seconds_now();

                if (!is_sorted_non_decreasing(work.data, work.n)) {
                    fprintf(stderr, "Sort failed for %s, case=%s, n=%d\n", ALGORITHMS[a].name, INPUT_FILES[c].name, n);
                    free(work.data);
                    fclose(csv);
                    return 1;
                }

                double elapsed = end - start;
                size_t aux_bytes = estimate_aux_memory_bytes(ALGORITHMS[a].name, work.n, min_value, max_value);

                fprintf(csv, "%s,%s,%d,%.9f,%zu\n", ALGORITHMS[a].name, INPUT_FILES[c].name, n, elapsed, aux_bytes);
                printf("%s | %s | n=%d -> %.9f seconds\n", ALGORITHMS[a].name, INPUT_FILES[c].name, n, elapsed);
                free(work.data);
            }
        }
    }

    fclose(csv);
    printf("Benchmark completed.\n");
    return 0;
}

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));

    if (argc >= 2 && strcmp(argv[1], "--benchmark") == 0) {
        int selected_size = 0;
        const char *csv_path = "benchmark_results.csv";

        if (argc >= 3) {
            char *endptr = NULL;
            long maybe_size = strtol(argv[2], &endptr, 10);
            if (endptr && *endptr == '\0' && maybe_size > 0) {
                selected_size = (int)maybe_size;
                if (argc >= 4) {
                    csv_path = argv[3];
                }
            } else {
                csv_path = argv[2];
            }
        }

        return run_benchmark_mode(csv_path, selected_size);
    }

    return run_competition_mode(argc, argv);
}
