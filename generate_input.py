#!/usr/bin/env python3
import argparse
import random
import sys


def build_values(count: int, low: int, high: int) -> list[int]:
    if count <= 0:
        raise ValueError("count must be > 0")
    if low > high:
        raise ValueError("min value cannot be greater than max value")
    if not (low < 0 < high):
        raise ValueError("range must include both negative and positive numbers (min < 0 < max)")

    values = [random.randint(low, high) for _ in range(count)]

    # Guarantee both signs exist when possible.
    if count >= 2:
        if not any(v < 0 for v in values):
            values[0] = random.randint(low, -1)
        if not any(v > 0 for v in values):
            values[-1] = random.randint(1, high)

    return values


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Generate integers (both positive and negative) and save them to in.txt"
    )
    parser.add_argument("count", type=int, help="Number of elements to generate")
    parser.add_argument("--min", dest="min_value", type=int, default=-1_000_000, help="Minimum value")
    parser.add_argument("--max", dest="max_value", type=int, default=1_000_000, help="Maximum value")
    parser.add_argument("--seed", type=int, default=None, help="Optional random seed")
    args = parser.parse_args()

    if args.seed is not None:
        random.seed(args.seed)

    try:
        values = build_values(args.count, args.min_value, args.max_value)
    except ValueError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    with open("in.txt", "w", encoding="utf-8") as f:
        for value in values:
            f.write(f"{value}\n")

    print(f"Generated {len(values)} values in in.txt")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())