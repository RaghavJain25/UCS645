UCS645: PARALLEL & DISTRIBUTED COMPUTING

Assignment 2: Performance Evaluation of OpenMP Programs using Parallel

1.Learning Objectives
• Implement basic OpenMP parallel programs.
• Measure execution time using omp_get_wtime().
• Compute speedup, efficiency, and cost metrics.
• Understand strong vs weak scaling using Amdahl’s and Gustafson’s
laws.
• Identify performance bottlenecks such as load imbalance,
synchronization overhead, false sharing, and memory bandwidth
saturation.
• Gain introductory exposure to performance profiling tools.

2.Experimental Environment
• Shared-memory multicore CPU.
• Operating System: Linux / Ubuntu (native or virtual machine).
• Compiler: gcc / g++ with OpenMP support.
• Number of threads: 1, 2, 4, 8 (or up to available cores).

🔹 Question 1: Molecular Dynamics – Lennard-Jones Force Calculation

Description:
This problem simulates a simplified molecular dynamics system where pairwise forces between particles are computed using the Lennard-Jones potential. The computation involves nested loops and shared data updates, making it suitable for studying synchronization overhead.

Key Aspects:

Parallelization of nested loops using OpenMP

Use of atomic operations to avoid race conditions

Reduction for total potential energy

Performance evaluation for different thread counts

Graphs Generated:

Execution Time vs Number of Threads

Speedup vs Number of Threads

🔹 Question 2: Bioinformatics – Smith–Waterman DNA Sequence Alignment

Description:
This problem implements the Smith–Waterman algorithm for local DNA sequence alignment. The algorithm is based on dynamic programming and contains data dependencies, which limit parallel scalability.

Key Aspects:

Wavefront (anti-diagonal) parallelization

Handling of data dependencies in dynamic programming

Evaluation of limited speedup due to algorithmic constraints

Graphs Generated:

Execution Time vs Number of Threads

Speedup vs Number of Threads

🔹 Question 3: Scientific Computing – Heat Diffusion Simulation

Description:
This problem simulates heat diffusion on a 2D grid using the finite difference method. It represents a typical stencil-based scientific computation with good spatial parallelism.

Key Aspects:

Parallelization of grid updates using OpenMP

Separation of old and new grids to avoid race conditions

Analysis of scalability for compute-intensive workloads

Graphs Generated:

Execution Time vs Number of Threads

Speedup vs Number of Threads


✅ Conclusion

This assignment demonstrates how different problem characteristics—such as synchronization overhead, data dependencies, and memory access patterns—affect the scalability of OpenMP programs. While compute-intensive problems like heat diffusion show good speedup, algorithms with inherent dependencies, such as Smith–Waterman, exhibit limited scalability. Overall, the assignment provides practical insight into parallel performance analysis on multicore systems.
