# Project 1 - Analysis of Sorting Algorithms

## Objectives
- Analyze and implement five sorting algorithms, including both efficient and less efficient ones.
- Evaluate the time complexity and memory usage of each algorithm under different input conditions.

## Implemented Algorithms
- Quick Sort
- Merge Sort
- Heap Sort
- Counting Sort
- Tim Sort

## Test Scenarios
Each algorithm is tested on:
- Randomly ordered elements
- Elements sorted in ascending order
- Elements sorted in descending order

## Input Sizes
The required sizes are:
- 100 elements
- 1,000 elements
- 10,000 elements
- 100,000 elements
- 1,000,000 elements

## Evaluation Criteria
For each algorithm and input type:
- Execution time
- Memory consumption

## Real-World Applications
- Quick Sort: fast in-memory sorting for general datasets.
- Merge Sort: stable sorting and external sorting pipelines.
- Counting Sort: integer keys with small value ranges.
- Heap Sort: predictable worst-case `O(n log n)` with low extra memory.
- Tim Sort: strong practical performance on partially sorted real data.

## Competition: The Fastest Sorting Algorithm

### Scenario
Sorting an array of 1,000,000 randomly generated elements.

### Performance Metrics
1. Computation time = sorting time only
2. Computation time = sorting time + file/console output time

### Program Requirements and Constraints
- Executable receives command-line parameters (`argc`, `argv`):
  - Input file (mandatory)
  - Output destination (optional: `stdout` or output file)
- Output rules:
  - No output destination: do not print/write sorted elements
  - `stdout`: print sorted elements to standard output
  - File name: write sorted elements to that file

## Build

### macOS/Linux (no CMake required)
```bash
cc -std=c11 main.c algorithms/*.c -o sort
```

### CMake (optional)
```bash
cmake -S . -B build
cmake --build build
```

## Run

Only these command formats are supported:
```bash
./sort quick in.txt
./sort merge in.txt stdout
./sort heap in.txt out.txt
./sort counting in.txt
./sort tim in.txt
```
Behavior:
- Runs a full report for the selected algorithm on 3 cases from the same input values:
  - `random/input_order`
  - `ascending`
  - `descending`
- Metrics:
  - no output target: `sorting only` + `peak memory consumption`
  - `stdout`: `sorting only` + `sorting + console output` + `peak memory consumption`
  - output file: `sorting only` + `sorting + file output` + `peak memory consumption`
- Output destination rules:
  - without destination: no sorted elements are printed or written
  - `stdout`: sorted elements are printed to terminal
  - file name (for example `out.txt`): sorted elements for all cases are written to that single file with `CASE:` headers

## Project structure
- `main.c`: CLI mode, case generation (`random/input_order`, `ascending`, `descending`), timing, memory reporting
- `operations.h`: sorting function declarations
- `algorithms/quick_sort.c`
- `algorithms/merge_sort.c`
- `algorithms/heap_sort.c`
- `algorithms/counting_sort.c`
- `algorithms/tim_sort.c`
- `CMakeLists.txt`
- `REPORT.md`
