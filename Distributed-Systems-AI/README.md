# Distributed Systems & AI Research 🤖

## 🌐 Module Overview
Advanced study of distributed computing architectures, synchronization algorithms, and privacy-preserving Artificial Intelligence. This module focuses on the **Actor Model** using **Erlang** and modern **Federated Learning** paradigms.

**Red Team Relevance:**
* **Concurrency bugs**: Understanding **Race Conditions** and Deadlocks in distributed environments is key for finding vulnerabilities in Cloud infrastructure.
* **Adversarial AI**: Federated Learning is the standard for privacy; understanding its architecture helps in designing attacks (Model Poisoning) or defenses.

## 📂 Distributed Algorithms (Erlang)

Implementation of fundamental distributed protocols without a central coordinator.

### 1. ⏱️ Logical Clocks & Mutual Exclusion
* **Code**: `lock_lamport.erl`, `muty.erl`
* **Concept**: Implementation of a **Distributed Mutex** (Lock) using **Lamport Logical Clocks** to order events in a decentralized system.
* **Challenge**: Solving Deadlocks and ensuring fairness among nodes without a master server.

### 2. 📣 Total Order Multicast (ISIS Algorithm)
* **Code**: `isis_algorithm.erl` (original `total.erl`)
* **Concept**: Implementation of the **ISIS algorithm** to guarantee that all nodes deliver multicast messages in the exact same order, regardless of network latency or jitter.
* **Skill**: Maintaining **Strong Consistency** in asynchronous networks.

### 3. 💬 Distributed Chat Server
* **File**: `Lab-1-Distributed-Chat-Architecture.pdf`
* **Concept**: Designing a fault-tolerant messaging system using Erlang's lightweight processes and message passing.

## 🧠 Federated Learning (AI Research)

Research on training AI models across decentralized edge devices without sharing data (Privacy-Preserving ML).

* **Simulation**: `Federated-Learning-Simulation.ipynb`
    * A Jupyter Notebook simulating a FL environment.
    * Comparison of **Local Training** vs **Federated Averaging (FedAvg)**.
    * Analysis of model convergence and bandwidth optimization.
* **Architecture**: `Federated-Learning-Architecture-Research.pdf`
    * Analysis of FL as a distributed operating system problem: Coordination, Consistency, and Fault Tolerance.

---
*Tech Stack: Erlang/OTP, Python (PyTorch/TensorFlow), Jupyter.*