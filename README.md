# AIA Sorting Algorithms Benchmark

Benchmark project for comparing 5 sorting algorithms on multiple input patterns and output scenarios.

## What this project does
- Implements and benchmarks:
  - Quick Sort
  - Merge Sort
  - Heap Sort
  - Counting Sort
  - Tim Sort
- Generates test datasets automatically for a selected `N`:
  - `random.txt`
  - `ascending.txt`
  - `descending.txt`
- Runs each algorithm on all 3 datasets.
- Measures:
  - sorting time only
  - sorting + console output time
  - sorting + file output time
  - peak memory usage (via Windows process API)

## Project structure
- `main.c`: dataset generation, benchmark orchestration, timing, memory reporting
- `operations.h`: sorting function declarations
- `algorithms/quick_sort.c`
- `algorithms/merge_sort.c`
- `algorithms/heap_sort.c`
- `algorithms/counting_sort.c`
- `algorithms/tim_sort.c`
- `CMakeLists.txt`: CMake build configuration
- `REPORT.md`: short theoretical/report notes

## Build

### Option 1: CLion
Open the folder and run the CMake target `main`.

### Option 2: CMake from terminal
```bash
cmake -S . -B build
cmake --build build
```

## Run
After build, run the executable and enter the input size when prompted:
- recommended sizes from the app prompt: `100`, `1000`, `10000`, `100000`, `1000000`

Example:
```bash
./build/main
```

Program prompt:
```text
Enter number of elements (100, 1000, 10000, 100000, 1000000):
```

## Output artifacts
- Generated input files in project root:
  - `random.txt`, `ascending.txt`, `descending.txt`
- Sorted output files (scenario 3):
  - `sorted_output_files/QuickSort_sorted.txt`
  - `sorted_output_files/MergeSort_sorted.txt`
  - `sorted_output_files/HeapSort_sorted.txt`
  - `sorted_output_files/CountingSort_sorted.txt`
  - `sorted_output_files/TimSort_sorted.txt`

## Important platform note
- `main.c` currently includes `windows.h` and `psapi.h` for peak memory reporting.
- This works directly on Windows.
- On macOS/Linux, compilation will fail unless memory-reporting code is adapted or conditionally compiled.
