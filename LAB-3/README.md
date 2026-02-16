# Parallel Correlation Assignment

## Description
This project computes the correlation coefficient between all pairs of input vectors.
The implementation supports both sequential and OpenMP-based parallel execution.

## Compilation
make

## Run
./correlation <rows> <cols>

Example:
./correlation 1000 1000

## Performance Measurement
Sequential:
OMP_NUM_THREADS=1 perf stat ./correlation 1000 1000

Parallel:
OMP_NUM_THREADS=4 perf stat ./correlation 1000 1000

Increase matrix size and thread count for performance evaluation.
