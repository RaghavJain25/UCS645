# CUDA Assignment 7 – Implementation and Analysis

## Introduction
This assignment explores CUDA programming concepts such as kernel execution, memory management, and parallel computation. The goal is to understand how GPU-based execution differs from CPU-based approaches through practical implementations.

---

##  Tools & Technologies Used
- CUDA Toolkit
- NVIDIA GPU / Google Colab (GPU runtime)
- C/C++ with CUDA
- Visual Studio Code

---

## Problem 1: Multi-Task Execution Using Threads

###  Objective
To design a CUDA program where different threads perform different tasks.

###  Approach
- Thread 0 computes the sum of first N integers using an iterative approach
- Thread 1 computes the same sum using the direct mathematical formula

###  Observation
Both approaches give the same result:
- Iterative method involves looping → relatively slower
- Formula method executes in constant time → faster

---

## Problem 2: Merge Sort Implementation

### (a) CPU-Based Merge Sort
- Implemented using recursive divide-and-conquer technique
- Executes sequentially

### (b) Parallel Merge Sort using CUDA
- Implemented using CUDA kernels
- Merging operations are distributed across threads
- Improves performance for larger datasets

### (c) Performance Comparison
- CPU approach is recursive and becomes slower as dataset size increases
- CUDA implementation uses parallel execution, making it faster for large inputs
- Pipelining helps overlap different stages of computation, improving overall efficiency

###  Insight
Parallel processing significantly enhances sorting performance when handling large data sizes.

---

##  Problem 3: Vector Addition and Performance Analysis

###  Features Implemented
- Basic vector addition kernel
- Kernel execution timing using CUDA events
- Device property query using `cudaDeviceProp`
- Theoretical memory bandwidth calculation
- Measured bandwidth calculation

### Observations
- Theoretical bandwidth represents maximum achievable performance under ideal conditions
- Measured bandwidth is lower due to real-world constraints like memory latency
- Efficient memory access patterns are important for better GPU performance

---

##  How to Run the Code

### Compile
```bash
nvcc problem1.cu -o p1
nvcc problem2.cu -o p2
nvcc problem3.cu -o p3
