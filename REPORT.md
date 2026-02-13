# Project 1 - Analysis of Sorting Algorithms

## Implemented Algorithms
- quick_sort (`quick.c`)
- merge_sort (`merge.c`)
- counting_sort (`counting.c`)
- heap_sort (`heap.c`)
- tim_sort (`timsort.c`)

All algorithms are called from `main.c`.

## Test Scenarios Covered
Benchmark mode (`--benchmark`) evaluates each algorithm on:
- randomly ordered elements
- ascending ordered elements
- descending ordered elements

## Input Sizes Covered
Benchmark mode runs the required sizes:
- 100
- 1,000
- 10,000
- 100,000
- 1,000,000

## Metrics Collected
For each algorithm x scenario x size, `benchmark_results.csv` stores:
- `execution_time_seconds`
- `estimated_aux_memory_bytes`

Memory estimates are based on algorithm auxiliary-space behavior:
- quick_sort: approximately `O(log n)` stack
- merge_sort: `O(n)` temporary buffers
- counting_sort: `O(n + k)` (`k` = value range)
- heap_sort: `O(1)` auxiliary space (plus recursion stack from heapify)
- tim_sort: `O(n)` temporary merge buffers

## Real-World Applications
- quick_sort: in-memory sorting for general datasets where average speed matters and worst-case is mitigated by pivot strategy.
- merge_sort: stable sorting for linked data and external sorting pipelines (large files/chunks).
- counting_sort: integer keys with limited range (exam scores, age buckets, category IDs).
- heap_sort: memory-constrained systems requiring guaranteed `O(n log n)` worst-case time.
- tim_sort: production-grade default for partially ordered real-world records (used in high-level language runtimes).

## Competition Mode (Fastest Sorting Algorithm)
`main.c` supports command-line format exactly as requested:

```bash
./sort in.txt
./sort in.txt stdout
./sort in.txt out.txt
```

Rules implemented:
- input file is mandatory
- output destination is optional
- without output destination: no sorted values are printed/written
- with `stdout`: sorted values printed to standard output
- with file name: sorted values written to that file

Reported metrics:
- Sorting time only
- Sorting + output time

Current selected competition algorithm in `main.c` is `tim_sort`.

## Analysis Mode
Run full required experiment set and export CSV:

```bash
./sort --benchmark benchmark_results.csv
```

If output path is omitted:

```bash
./sort --benchmark
```

Default output file: `benchmark_results.csv`.
