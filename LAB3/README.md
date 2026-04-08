#  Optimized Correlation Matrix Engine

---

##  Overview

This project develops an optimized system to compute the **Pearson Correlation Coefficient** between all pairs of rows in a matrix.

The focus of this implementation is performance engineering through:

- Algorithmic restructuring
- Multi-threading with OpenMP
- SIMD-level vectorization
- Cache-aware memory access
- Empirical benchmarking using `perf`

The optimized implementation achieves a measured peak throughput of **16.97 GFLOPS** on a 500 × 500 matrix.

---

## Performance Summary (500 × 500 Matrix)

| Execution Mode        | GFLOPS | Speedup | Parallel Efficiency | Remarks |
|-----------------------|--------|----------|----------------------|----------|
| Sequential Version    | 4.49   | 1.00×    | 100%                 | Baseline |
| OpenMP (2 Threads)    | 9.16   | 2.01×    | ~100%                | Ideal Scaling |
| OpenMP (4 Threads)    | 16.97  | 3.73×    | 93.4%                | Best Result |
| OpenMP (8 Threads)    | 11.34  | 2.53×    | 31.7%                | Resource Contention |

---

##  Optimization Approach

### 1️ Pre-Processing Strategy (Normalization First)

Instead of repeatedly applying the full Pearson formula for each row pair,  
the matrix is normalized in advance:

- Each row is mean-centered
- Each row is scaled to unit variance

After this transformation, correlation reduces to a **simple dot product**, which dramatically simplifies the inner loop.

### Why This Matters

- Reduces arithmetic redundancy
- Improves vectorization potential
- Minimizes hot-loop complexity
- Enhances numerical stability

---

### 2️ Parallel Execution using OpenMP

Parallelization is applied across the correlation computation phase.

Key design choices:

- Flattened iteration space for improved workload distribution
- Dynamic scheduling to handle uneven triangular workload
- Thread count experimentation to determine optimal scaling

Example pragma used:

```cpp
#pragma omp parallel for schedule(dynamic)
```

---

### 3️ Micro-Architectural Tuning

To extract maximum CPU performance:

- Enabled aggressive optimization flags
- Used `#pragma omp simd` for vectorized dot products
- Ensured row-major traversal to preserve spatial locality
- Reduced unnecessary memory loads

This combination helps maximize:

- L1/L2 cache utilization
- Instruction-level parallelism
- AVX/AVX2 vector throughput

---

##  Scalability Analysis

Although the system exposes 8 logical cores, experimental results indicate optimal performance at 4 threads.

### Observations

- Near-linear scaling up to 4 threads (~93% efficiency)
- Noticeable degradation at 8 threads
- Efficiency drop suggests memory bandwidth limitations
- Hyper-threading contention affects compute units

This highlights the importance of understanding hardware topology when designing parallel programs.

---

##  Build Environment

- **Operating System:** WSL2 (Ubuntu 22.04)
- **Compiler:** g++ 11.4.0
- **Optimization Flags:**

```
-O3 -fopenmp -ffast-math -march=native
```

---

##  Compilation

```
make
```

---

##  Execution

```
./correlation <rows> <cols>
```

Example:

```
./correlation 500 500
```

---

##  Performance Measurement

Sequential run:

```
OMP_NUM_THREADS=1 perf stat ./correlation 500 500
```

Parallel run:

```
OMP_NUM_THREADS=4 perf stat ./correlation 500 500
```

---

##  Performance Metrics

Speedup is computed as:

```
Speedup = Sequential_Time / Parallel_Time
```

Parallel efficiency:

```
Efficiency = Speedup / Number_of_Threads
```

---

##  Final Remarks

This implementation demonstrates:

- Effective algorithm redesign
- Practical multi-threaded scaling
- Hardware-conscious optimization
- Quantitative performance evaluation

The results confirm that peak efficiency is bounded by hardware constraints rather than algorithmic limitations.

---
