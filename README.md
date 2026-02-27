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

### macOS/Linux (GCC/Clang, no CMake)
```bash
cc -O2 -std=c11 main.c algorithms/*.c -o sort
```

### macOS/Linux (CMake, optional)
```bash
cmake -S . -B build
cmake --build build
```

### Windows (MinGW-w64 GCC, PowerShell or `cmd`)
```bash
gcc -O2 -std=c11 main.c algorithms/quick_sort.c algorithms/merge_sort.c algorithms/heap_sort.c algorithms/counting_sort.c algorithms/tim_sort.c -o sort.exe
```

### Windows (CMake, optional)
```powershell
cmake -S . -B build
cmake --build build --config Release
```

### Generate input elements (`in.txt`)
Use the helper script after building to create test data.

```bash
python3 generate_input.py 100000 --min -1000000 --max 1000000 --seed 42
```

Windows PowerShell example:
```powershell
python .\generate_input.py 100000 --min -1000000 --max 1000000 --seed 42
```

## Run

Supported command formats (same arguments on all platforms):
```text
<program> <algo> in.txt
<program> <algo> in.txt stdout
<program> <algo> in.txt out.txt
<program> <algo1> <algo2> ... in.txt
<program> all in.txt
```

### macOS/Linux examples
```bash
./sort quick in.txt
./sort merge in.txt stdout
./sort heap in.txt out.txt
./sort counting in.txt
./sort tim in.txt
./sort quick merge heap in.txt
./sort all in.txt
```

### Windows PowerShell examples
```powershell
.\sort.exe quick in.txt
.\sort.exe merge in.txt stdout
.\sort.exe heap in.txt out.txt
.\sort.exe counting in.txt
.\sort.exe tim in.txt
.\sort.exe quick merge heap in.txt
.\sort.exe all in.txt
```

### Windows Command Prompt (`cmd`) examples
```cmd
sort.exe quick in.txt
sort.exe merge in.txt stdout
sort.exe heap in.txt out.txt
sort.exe counting in.txt
sort.exe tim in.txt
sort.exe quick merge heap in.txt
sort.exe all in.txt
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
