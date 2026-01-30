# Data Structures & Algorithms (C++) 🧩

## ⚡ Module Overview
Deep dive into algorithmic efficiency, memory management, and advanced data structures using **C++ (STL)**. The focus is on analyzing time/space complexity (**Big O Notation**) and implementing optimized storage mechanisms like Hash Tables, AVLs, and Tries.

**Red Team Relevance:**
* **Memory Management:** Debugging errors like `Segmentation Fault` or `free(): invalid size` (mentioned in the reports) provides the necessary background to understand **Memory Corruption Vulnerabilities** (Buffer Overflows, Heap exploitation).
* **Tool Optimization:** Knowledge of efficient data structures (Tries, Hash Maps) is essential for developing high-speed security tools (e.g., password crackers, fuzzers).

## 📂 Projects

### 1. 📖 Dictionary & Anagram Engine
* **File**: `Project-Dictionary-Optimization-Tries.pdf`
* **Goal**: Build an optimized dictionary system to find anagrams and words efficiently.
* **Key Structures Implemented**:
    * **Tries (Prefix Trees)** & **TST (Ternary Search Trees)**: Implemented to optimize prefix lookups vs standard `std::map`.
    * **Hash Tables**: For O(1) average access time.
* **Challenges Solved**:
    * **Memory Leaks**: Manual management of dynamic memory (pointers, destructors).
    * **Optimization**: Refactoring code to avoid `Segmentation Faults` and improve execution time on large datasets.

### 2. 🧮 Algorithmic Complexity Analysis
* **File**: `Lab-Anagram-Solver-Complexity.pdf`
* **Skills**:
    * **Canonical Anagrams**: Sorting strings to find permutations.
    * **Big O Analysis**: Calculating the theoretical cost of algorithms (e.g., *Θ(n log n)* for sorting-based approaches).
    * **Recursion**: Solving subset generation problems.

---
*Tech Stack: C++, STL, Valgrind, GDB.*