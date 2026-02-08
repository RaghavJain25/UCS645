# UCS645: Parallel & Distributed Computing  
## Assignment 2 – Performance Evaluation of OpenMP Programs

---

## 📌 Assignment Overview
This assignment focuses on the performance evaluation of parallel programs using **OpenMP** on shared-memory multicore systems. The objective is to study how execution time and scalability change with varying numbers of threads and to understand the impact of different computational patterns on parallel performance.

Three representative problems from scientific computing and bioinformatics are implemented and analyzed. For each problem, execution time and speedup are measured and visualized using performance graphs.

---

## 🎯 Learning Objectives
- Implement basic OpenMP parallel programs  
- Measure execution time using `omp_get_wtime()`  
- Compute speedup, efficiency, and cost metrics  
- Understand strong vs weak scaling  
- Apply Amdahl’s Law and Gustafson’s Law  
- Identify performance bottlenecks such as:
  - Load imbalance  
  - Synchronization overhead  
  - Data dependencies  
  - Memory access limitations  
- Gain introductory exposure to performance analysis  

---

## 🖥 Experimental Environment
- **Architecture:** Shared-memory multicore CPU  
- **Operating System:** Linux / Ubuntu (native or VM / WSL)  
- **Compiler:** gcc / g++ with OpenMP support  
- **Parallel Programming Model:** OpenMP  
- **Number of Threads:** 1, 2, 4, 8 (or up to available cores)  

---

## 🛠 Tools & Technologies
- **Language:** C++  
- **Parallel Library:** OpenMP  
- **Timing Function:** `omp_get_wtime()`  
- **Graph Plotting:** gnuplot  
- **Compiler Flags:**  
  ```bash
  g++ -O3 -fopenmp filename.cpp -o output
---
## 🔹 Question 1: Molecular Dynamics – Lennard-Jones Force Calculation

### Description
This problem simulates a simplified molecular dynamics system where pairwise forces between particles are computed using the **Lennard-Jones potential**. The computation involves nested loops and shared data updates, making it suitable for studying synchronization overhead in parallel programs.

### Key Aspects
- Parallelization of nested loops using OpenMP  
- Use of atomic operations to prevent race conditions  
- Reduction for computing total potential energy  
- Performance evaluation across multiple thread counts  

### Graphs Generated
- Execution Time vs Number of Threads  
- Speedup vs Number of Threads  

---

## 🔹 Question 2: Bioinformatics – Smith–Waterman DNA Sequence Alignment

### Description
This problem implements the **Smith–Waterman algorithm** for local DNA sequence alignment. The algorithm is based on dynamic programming and contains inherent data dependencies that limit parallel scalability.

### Key Aspects
- Wavefront (anti-diagonal) parallelization  
- Handling of data dependencies in dynamic programming  
- Analysis of limited speedup due to algorithmic constraints  

### Graphs Generated
- Execution Time vs Number of Threads  
- Speedup vs Number of Threads  

---

## 🔹 Question 3: Scientific Computing – Heat Diffusion Simulation

### Description
This problem simulates **heat diffusion on a 2D grid** using the finite difference method. It represents a stencil-based scientific computation with good spatial parallelism.

### Key Aspects
- Parallelization of grid updates using OpenMP  
- Use of separate grids to avoid race conditions  
- Scalability analysis for compute-intensive workloads  

### Graphs Generated
- Execution Time vs Number of Threads  
- Speedup vs Number of Threads

---

## ✅ Conclusion

This assignment demonstrates how different problem characteristics—such as synchronization overhead, data dependencies, and memory access patterns—affect the scalability of OpenMP programs. Compute-intensive problems like **heat diffusion** show better speedup due to higher parallel efficiency, while algorithms with inherent data dependencies, such as **Smith–Waterman**, exhibit limited scalability. Overall, this assignment provides practical insight into parallel performance evaluation and scalability analysis on multicore systems using OpenMP.
