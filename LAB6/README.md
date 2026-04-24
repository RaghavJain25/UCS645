#  CUDA Assignment 6 – 

---

## Aim

To study CUDA programming and implement:

* Device Query
* Parallel Array Sum
* Matrix Addition using GPU

---

##  Theory

CUDA (Compute Unified Device Architecture) is a parallel computing platform that enables GPUs to perform general-purpose computation. It uses a hierarchy of:

* Threads
* Blocks
* Grids

---

# PART A: DEVICE QUERY

---

##  Answers

### 1. Architecture & Compute Capability

Depends on GPU (example: 7.5 – Turing)

---

### 2. Maximum Block Dimensions

Example: `1024 x 1024 x 64`

---

### 3. Maximum Threads

```
65535 × 512 = 33,553,920 threads
```

---

### 4. Why not use maximum threads?

* Memory limits
* Inefficiency
* Thread divergence

---

### 5. What limits threads?

* Registers
* Shared memory
* Hardware limits

---

### 6. Shared Memory

Fast memory inside block (≈ 48 KB)

---

### 7. Global Memory

Large GPU memory (GBs)

---

### 8. Constant Memory

Read-only memory (~64 KB)

---

### 9. Warp Size

Group of threads (usually 32)

---

### 10. Double Precision

Supported (depends on GPU)

---

#  PART B: ARRAY SUM USING CUDA

---

##  Steps Performed

1. Allocate device memory
2. Copy data to GPU
3. Define grid/block
4. Execute kernel
5. Copy result back
6. Free memory

---

#  PART C: MATRIX ADDITION


---

##  Answers

### 1. Floating Operations

```
N × N additions
```

---

### 2. Global Memory Reads

```
2 × N × N
```

---

### 3. Global Memory Writes

```
N × N
```

---

#  Results

* Correct GPU execution observed
* Faster than CPU for large inputs
* Efficient parallel computation

---

#  Challenges

* CUDA installation
* Debugging kernels
* Memory handling

---

#  Conclusion

CUDA allows efficient parallel execution on GPU. This assignment demonstrated:

* GPU architecture understanding
* Memory management
* Performance improvement using parallel computing



