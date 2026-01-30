# Operating Systems Internals (Linux) 🐧

## ⚙️ Module Overview
Advanced system programming in **C** focusing on the **Linux Kernel API**. This module covers the fundamental mechanisms of the operating system: process management, memory allocation, inter-process communication (IPC), and network sockets.

**Red Team Relevance:**
Understanding **Syscalls** (`fork`, `exec`, `ptrace`) and memory management is the foundation for **Malware Development**, **Process Injection**, and Evasion techniques.

## 📂 Labs & Challenges

### 1. 🏆 The Challenge: Client-Server Architecture
* **File**: `Advanced-System-Programming-Sockets.pdf`
* **Description**:
    * Development of a robust client-server application using **TCP Sockets**.
    * **Key Skills**:
        * Handling **Race Conditions** with atomic signals (`sig_atomic_t`).
        * Process synchronization between Parent/Child processes.
        * Robust error handling for network syscalls.

### 2. 📡 Inter-Process Communication (IPC)
* **File**: `IPC-Signal-Handling.pdf` (Lab 4)
* **Technique**:
    * Using **Signals** (`SIGUSR1`, `SIGINT`) to coordinate actions between processes.
    * Implementing custom Signal Handlers to modify process behavior (a technique often used to prevent malware termination).

### 3. 🧬 Process Management & Syscalls
* **File**: `Process-Creation-Fork-Exec.pdf` (Lab 1)
* **Concepts**:
    * Deep dive into `fork()` for process cloning and `exec()` family to replace process images.
    * Understanding the Process Control Block (PCB) and PID management.

### 4. 💾 Low-Level I/O & Buffers
* **File**: `Syscalls-IO-Buffer-Management.pdf` (Lab 2)
* **Concepts**:
    * Direct manipulation of File Descriptors (FD) using `read`/`write` syscalls.
    * Buffer management logic (essential for understanding Buffer Overflows).

---
*Tech Stack: C, Linux API, GCC, Bash, Makefiles.*