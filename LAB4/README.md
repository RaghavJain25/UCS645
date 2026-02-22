
## Assignment 4 – Introduction to MPI  

---

# 📌 Objective

This assignment introduces Message Passing Interface (MPI) programming concepts including:

- Process ranks and communicators  
- Point-to-point communication  
- Collective communication  
- Parallel computation  
- Performance measurement  
- Implementation of 4 laboratory exercises  

---

# 🖥️ System Requirements

- Ubuntu / Linux
- MPICH or OpenMPI installed

## Install MPI (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install mpich
mpicc --version
```

---

# ⚙️ Compilation & Execution

To compile:

```bash
mpicc -o program_name program_name.c
```

To run:

```bash
mpirun -np <number_of_processes> ./program_name
```

Example:

```bash
mpirun -np 4 ./ring_comm
```

---

# 🔹 Exercise 1: Ring Communication

## Problem Statement

- Process 0 starts with value 100  
- Each process adds its rank  
- Message moves in ring topology  
- Final value returns to Process 0  

## Steps

1. Save file as `ring_comm.c`
2. Compile: `mpicc -o ring_comm ring_comm.c`
3. Run: `mpirun -np 4 ./ring_comm`

## Output (4 processes)

```
Process 0 starting with value 100
Process 1 received and updated value to 101
Process 2 received and updated value to 103
Process 3 received and updated value to 106
Process 0 received final value 106
```

---

# 🔹 Exercise 2: Parallel Array Sum

## Problem Statement

- Create array 1–100  
- Scatter among processes  
- Compute local sums  
- Use MPI_Reduce  
- Expected sum = 5050  

## Steps

1. Save file as `array_sum.c`
2. Compile: `mpicc -o array_sum array_sum.c`
3. Run: `mpirun -np 4 ./array_sum`

## Output

```
Global Sum = 5050
Average = 50.50
```

---

# 🔹 Exercise 3: Global Maximum & Minimum

## Problem Statement

- Each process generates 10 random numbers  
- Find global maximum & minimum  
- Use MPI_Reduce with MPI_MAX and MPI_MIN  

## Steps

1. Save file as `max_min.c`
2. Compile: `mpicc -o max_min max_min.c`
3. Run: `mpirun -np 4 ./max_min`

## Output

```
Global Maximum = 987
Global Minimum = 12
```

(Note: Values vary due to randomness)

---

# 🔹 Exercise 4: Parallel Dot Product

## Problem Statement

Vector A = [1,2,3,4,5,6,7,8]  
Vector B = [8,7,6,5,4,3,2,1]  

Expected Result = 120  

## Steps

1. Save file as `dot_product.c`
2. Compile: `mpicc -o dot_product dot_product.c`
3. Run: `mpirun -np 4 ./dot_product`

##  Output

```
Dot Product = 120
```

---

# 📊 Performance Measurement

## Commands

```bash
mpicc -O3 -o test program.c
mpirun -np 1 ./test
mpirun -np 2 ./test
mpirun -np 4 ./test
mpirun -np 8 ./test
```

---

# 📈 Result Table (Sample)

| Processes (p) | Time (Tp) | Speedup (Sp) | Efficiency (Ep) |
|---------------|-----------|--------------|-----------------|
| 1 | 8.0 sec | 1.00 | 1.00 |
| 2 | 4.2 sec | 1.90 | 0.95 |
| 4 | 2.3 sec | 3.47 | 0.86 |
| 8 | 1.5 sec | 5.33 | 0.66 |

---

# 📌 Formulas

Speedup:

Sₚ = T₁ / Tₚ  

Efficiency:

Eₚ = Sₚ / p  

---

# 📌 Observations

- Speedup increases with number of processes  
- Efficiency decreases due to communication overhead  
- Moderate process count gives best performance  
- Communication cost affects scalability  

---

# ✅ Conclusion

This assignment demonstrates:

- MPI process communication  
- Collective operations (Scatter, Reduce)  
- Parallel computation  
- Performance analysis  
- Scalability concepts  

MPI enables efficient distributed memory parallel programming and is widely used in high-performance computing applications.
