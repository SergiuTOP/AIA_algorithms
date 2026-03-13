#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INSERTION_THRESHOLD 16

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static inline void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static void insertion_sort(int *arr, int lo, int hi) {
    for (int i = lo + 1; i <= hi; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= lo && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

static void quicksort(int *arr, int lo, int hi) {
    while (hi - lo >= INSERTION_THRESHOLD) {
        int mid = lo + ((hi - lo) >> 1);
        if (arr[lo] > arr[mid]) swap(&arr[lo], &arr[mid]);
        if (arr[lo] > arr[hi]) swap(&arr[lo], &arr[hi]);
        if (arr[mid] > arr[hi]) swap(&arr[mid], &arr[hi]);

        swap(&arr[mid], &arr[hi - 1]);
        int pivot = arr[hi - 1];

        int i = lo;
        int j = hi - 1;
        for (;;) {
            while (arr[++i] < pivot) {
            }
            while (arr[--j] > pivot) {
            }
            if (i >= j) {
                break;
            }
            swap(&arr[i], &arr[j]);
        }

        swap(&arr[i], &arr[hi - 1]);

        if (i - lo < hi - i) {
            quicksort(arr, lo, i - 1);
            lo = i + 1;
        } else {
            quicksort(arr, i + 1, hi);
            hi = i - 1;
        }
    }

    insertion_sort(arr, lo, hi);
}

static int write_sorted(FILE *out, const int *arr, int n) {
    size_t cap = (size_t)n * 12u + 1u;
    char *obuf = (char *)malloc(cap);
    if (!obuf) {
        return 0;
    }

    size_t pos = 0;
    for (int i = 0; i < n; i++) {
        long long x = (long long)arr[i];
        if (x < 0) {
            obuf[pos++] = '-';
            x = -x;
        }

        if (x == 0) {
            obuf[pos++] = '0';
        } else {
            char digits[20];
            int dlen = 0;
            while (x > 0) {
                digits[dlen++] = (char)('0' + (x % 10));
                x /= 10;
            }
            while (dlen > 0) {
                obuf[pos++] = digits[--dlen];
            }
        }
        obuf[pos++] = '\n';
    }

    size_t written = fwrite(obuf, 1, pos, out);
    free(obuf);
    return written == pos;
}

static void print_usage(const char *program_name) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s in.txt\n", program_name);
    fprintf(stderr, "  %s in.txt stdout\n", program_name);
    fprintf(stderr, "  %s in.txt out.txt\n", program_name);
}

int main(int argc, char *argv[]) {
    const char *input_path = NULL;
    const char *output_target = NULL;
    if (argc < 2 || argc > 3) {
        print_usage(argv[0]);
        return 1;
    }
    input_path = argv[1];
    if (argc == 3) {
        output_target = argv[2];
    }

    FILE *fin = fopen(input_path, "rb");
    if (!fin) {
        perror(input_path);
        return 1;
    }

    if (fseek(fin, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(fin);
        return 1;
    }
    long fsize = ftell(fin);
    if (fsize < 0) {
        perror("ftell");
        fclose(fin);
        return 1;
    }
    rewind(fin);

    char *buf = (char *)malloc((size_t)fsize + 1u);
    if (!buf) {
        fprintf(stderr, "Memory allocation failed while reading input.\n");
        fclose(fin);
        return 1;
    }
    size_t bytes_read = fread(buf, 1, (size_t)fsize, fin);
    buf[bytes_read] = '\0';
    fclose(fin);

    int cap = 1 << 20;
    int *arr = (int *)malloc((size_t)cap * sizeof(int));
    if (!arr) {
        fprintf(stderr, "Memory allocation failed for numbers array.\n");
        free(buf);
        return 1;
    }

    int n = 0;
    char *p = buf;
    while (*p) {
        while (*p && ((*p < '0' || *p > '9') && *p != '-')) {
            p++;
        }
        if (!*p) {
            break;
        }

        int sign = 1;
        if (*p == '-') {
            sign = -1;
            p++;
        }

        int val = 0;
        while (*p >= '0' && *p <= '9') {
            val = val * 10 + (*p - '0');
            p++;
        }

        if (n == cap) {
            int new_cap = cap * 2;
            int *tmp = (int *)realloc(arr, (size_t)new_cap * sizeof(int));
            if (!tmp) {
                fprintf(stderr, "Memory reallocation failed for numbers array.\n");
                free(arr);
                free(buf);
                return 1;
            }
            arr = tmp;
            cap = new_cap;
        }

        arr[n++] = sign * val;
    }
    free(buf);

    if (n == 0) {
        fprintf(stderr, "Input file has no valid integers: %s\n", input_path);
        free(arr);
        return 1;
    }

    double t1 = now_seconds();
    if (n > 1) {
        quicksort(arr, 0, n - 1);
    }
    double t2 = now_seconds();
    double sorting_only_time = t2 - t1;

    double sorting_plus_output_time = 0.0;
    if (output_target) {
        FILE *fout = NULL;
        int output_is_stdout = strcmp(output_target, "stdout") == 0;
        if (output_is_stdout) {
            fout = stdout;
        } else {
            fout = fopen(output_target, "wb");
            if (!fout) {
                perror(output_target);
                free(arr);
                return 1;
            }
        }

        double t3 = now_seconds();
        if (!write_sorted(fout, arr, n)) {
            fprintf(stderr, "Failed to write sorted output.\n");
            if (!output_is_stdout) {
                fclose(fout);
            }
            free(arr);
            return 1;
        }
        if (!output_is_stdout) {
            fclose(fout);
        }
        double t4 = now_seconds();
        sorting_plus_output_time = sorting_only_time + (t4 - t3);
    }

    fprintf(stderr, "========================================\n");
    fprintf(stderr, "Input file:            %s\n", input_path);
    fprintf(stderr, "Elements sorted:       %d\n", n);
    fprintf(stderr, "Sorting time:          %.6f sec\n", sorting_only_time);
    if (output_target) {
        if (strcmp(output_target, "stdout") == 0) {
            fprintf(stderr, "Total (sort+stdout):   %.6f sec\n", sorting_plus_output_time);
        } else {
            fprintf(stderr, "Total (sort+file):     %.6f sec\n", sorting_plus_output_time);
        }
    }
    fprintf(stderr, "========================================\n");

    free(arr);
    return 0;
}
