# Parallel Computing & HPC 🚀

## ⚡ Module Overview
Focus on **High Performance Computing (HPC)** and shared-memory parallel programming using **C/C++** and **OpenMP**. The goal is to optimize algorithms to exploit multi-core architectures, avoiding **Race Conditions** and analyzing **Amdahl's Law**.

**Red Team Relevance:**
Mastering concurrency and C programming is essential for **Exploit Development**, writing high-speed scanners, and understanding complex memory race conditions (e.g., *Dirty Cow* vulnerability logic).

## 📂 Source Code & Projects

### 1. 🤖 Parallel AI: K-Means Clustering
* **Folder**: `Project-Parallel-KMeans`
* **File**: `Parallel-K-Means-Clustering-Algorithm.pdf`
* **Description**: Implementation of the K-Means unsupervised learning algorithm using atomic operations for thread-safe accumulation.

### 2. 🌌 The Mandelbrot Set Optimization (C/C++)
A progressive evolution of code optimization strategies.

* **Phase 1: Recursive Strategy** (`Lab-3`)
    * **Code**: `mandel-rec-tree.c`
    * **Technique**: Recursive decomposition using a Quadtree approach to manage computational load.
* **Phase 2: Fine-Grain Load Balancing** (`Lab-4`)
    * **Code**: `mandel-omp-iter-finegrain.c`
    * **Technique**: Dynamic scheduling to solve the "workload imbalance" issue in irregular datasets.
* **Phase 3: Geometric Decomposition** (`Lab-5`)
    * **Code**: `mandel-omp-iter-cas3.cpp`
    * **Technique**: Block-Cyclic distribution to optimize memory locality and reduce cache misses.

### 3. 🛠️ OpenMP Fundamentals
* **Folders**: `Lab-1` & `Lab-2`
* **Key Concepts**:
    * **Race Conditions**: Analysis of synchronization failures and fixes using `#pragma omp critical` and `atomic`.
    * **Task Decomposition**: Managing overhead vs. granularity.

---
*Tech Stack: C, C++, OpenMP, Linux GCC, Tareador Analysis Tool.*