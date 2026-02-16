⚡ High-Performance Correlation Matrix Computation
UCS645: Parallel and Distributed Computing | Lab Assignment 3
This project implements a high-performance engine to compute the Pearson Correlation Coefficient for matrix row pairs. By leveraging OpenMP, SIMD vectorization, and algorithmic pre-normalization, the implementation achieves a peak throughput of 16.97 GFLOPS.

🚀 Performance Highlights (500×500 Matrix)
Configuration	Throughput (GFLOPS)	Speedup	Efficiency	Status
Sequential Baseline	4.49	1.00x	100%	Reference
OpenMP (2 Threads)	9.16	2.01x	100.5%	Ideal Scaling
OpenMP (4 Threads)	16.97	3.73x	93.4%	⭐ Peak Performance
OpenMP (8 Threads)	11.34	2.53x	31.7%	Oversubscribed
🛠️ Optimization Stack
1. Algorithmic: Pre-Normalization Strategy
Instead of calculating correlation for every pair using the standard 
O
(
N
2
)
 formula, we pre-normalize the entire matrix (
m
e
a
n
=
0
, 
s
t
d
d
e
v
=
1
).

Impact: Reduces the core correlation logic to a simple Dot Product, significantly lowering the number of operations inside the hot loop.
2. Parallelism: OpenMP Orchestration
Nested Loop Collapsing: Used #pragma omp parallel for collapse(2) to flatten the triangular iteration space, providing a larger pool of work for threads.
Dynamic Load Balancing: Implemented schedule(dynamic) to handle the decreasing workload of the lower triangular matrix, ensuring no thread remains idle.
3. Hardware-Level: SIMD & Cache Tuning
SIMD Vectorization: Leveraged #pragma omp simd to enable AVX/AVX2 instruction sets for the dot product calculations.
Cache Locality: Optimized memory access patterns using row-major traversal to maximize L1/L2 cache hit rates.
📊 Deep-Dive Analysis
The 4-Thread "Sweet Spot"
As a Bachelor of Engineering student at Thapar, it's critical to note the hardware-software ceiling. While the system reports 8 logical cores, peak efficiency was reached at 4 threads. [Image of a Parallel Efficiency graph showing the drop-off after the physical core limit]

Observation: Scaling is near-linear up to 4 threads (
93.4
 efficiency).
Bottleneck: At 8 threads, performance degrades to 
31.7
 efficiency. This suggests Memory Bandwidth Saturation and contention between Hyper-Threads (SMT) sharing execution units.
⚙️ Compilation & Benchmarking
Environment
OS: WSL2 (Ubuntu 22.04)
Compiler: g++ 11.4.0
Flags: -O3 -fopenmp -ffast-math -march=native
Build and Run
# Clone and Navigate
cd lab_3/

# Build with Aggressive Optimization
make clean && make

# Execute Benchmark (Row x Column)
./correlate 500 500
