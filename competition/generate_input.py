#!/usr/bin/env python3
import argparse
import random
from pathlib import Path


def build_values(count: int, low: int, high: int) -> list[int]:
    if count <= 0:
        raise ValueError("count must be > 0")
    if low > high:
        raise ValueError("min cannot be greater than max")
    return [random.randint(low, high) for _ in range(count)]


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Generate random integers for competition Quick Sort input."
    )
    parser.add_argument("count", nargs="?", type=int, default=1_000_000, help="Number of integers to generate")
    parser.add_argument("--min", dest="min_value", type=int, default=-1_000_000, help="Minimum value")
    parser.add_argument("--max", dest="max_value", type=int, default=1_000_000, help="Maximum value")
    parser.add_argument("--seed", type=int, default=None, help="Optional random seed")
    parser.add_argument(
        "--output",
        type=Path,
        default=Path(__file__).resolve().parent / "in.txt",
        help="Output file path (default: competition/in.txt)",
    )
    args = parser.parse_args()

    if args.seed is not None:
        random.seed(args.seed)

    values = build_values(args.count, args.min_value, args.max_value)
    args.output.parent.mkdir(parents=True, exist_ok=True)

    with args.output.open("w", encoding="utf-8") as f:
        for value in values:
            f.write(f"{value}\n")

    print(f"Generated {len(values)} values in {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
