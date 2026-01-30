# Computer Architecture & Assembly Language ⚙️

## 🧠 Module Overview
Deep dive into the hardware-software interface, focusing on the **SISA-F Architecture** (RISC-based). This module covers low-level programming, memory hierarchy, and interrupt handling.

**Why this matters for Security:**
Understanding how CPU registers, the stack, and memory addressing work is the prerequisite for **Reverse Engineering**, **Exploit Development**, and writing **Shellcode**.

## 📂 Lab Reports & Projects

### 1. 📟 I/O & Memory Mapped Devices
* **File**: `Assembly-IO-Display-Drivers.pdf`
* **Skills**:
    * Direct manipulation of **Memory-Mapped I/O** (MMIO) to control display output.
    * Implementation of basic graphics drivers in Assembly.
    * Register management (`R0`-`R7`) and arithmetic logic.

### 2. ⏱️ Interrupts & Peripherals
* **File**: `Interrupts-Timer-Keyboard-Management.pdf`
* **Skills**:
    * Writing **Interrupt Service Routines (ISR)** for keyboard input and system clock.
    * Understanding the CPU trap mechanism and context switching.
    * Synchronization between hardware events and software logic.

### 3. 💾 Cache Memory & Optimization
* **File**: `Memory-Cache-Optimization-Analysis.pdf`
* **Skills**:
    * Translating high-level C code (Loops/Arrays) into optimized Assembly.
    * **Cache Hit/Miss Analysis**: Understanding spatial and temporal locality to optimize data access.
    * Stack pointer management and function calling conventions.

---
*Environment: SISA-F Simulator / Assembly*