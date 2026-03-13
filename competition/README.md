# Competition Runner

This folder contains the competition implementation that uses **Quick Sort only** on the input values from `in.txt`.

## Files

- `vanea.c` - C program for sorting and timing.
- `generate_input.py` - Python generator for `in.txt`.

## Generate Input

From the `competition/` folder:

```bash
python3 generate_input.py 1000000 --seed 42
```

This creates `in.txt` with random integers (default range: `[-1000000, 1000000]`).

## Build

```bash
cc -O2 -std=c11 vanea.c -o vanea
```

## Run

```bash
./vanea in.txt
./vanea in.txt stdout
./vanea in.txt out.txt
```

## Output Rules

- `./vanea in.txt`:
  - no sorted values are printed or written
  - timing is printed to `stderr`
- `./vanea in.txt stdout`:
  - sorted values are printed to terminal
  - timing is printed to `stderr`
- `./vanea in.txt out.txt`:
  - sorted values are written to `out.txt`
  - timing is printed to `stderr`
