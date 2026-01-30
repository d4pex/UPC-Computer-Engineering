# Artificial Intelligence & Algorithmic Problem Solving (Java) 🧠

## ☕ Module Overview
Advanced programming in **Java** focused on **Artificial Intelligence (GOFAI)**, data structures, and algorithmic efficiency. The projects demonstrate the ability to implement complex search strategies and adversarial game logic from scratch without external AI libraries.

**Red Team Relevance:**
* **Graph Theory**: Algorithms like **BFS** and **DFS** are the mathematical foundation of **Attack Path Mapping** tools (e.g., BloodHound in Active Directory).
* **Reverse Engineering**: Deep familiarity with Java project structures (`pom.xml`, package hierarchy, `.jar` building) is essential for analyzing enterprise applications and deserialization vulnerabilities.

## 📂 Projects

### 1. ♟️ Strategic Game AI: "Oust"
* **Folder**: `Project-Oust-Board-Game-AI`
* **Description**: Implementation of a full strategic board game with a GUI and autonomous agents.
* **Key Algorithms**:
    * **Minimax with Alpha-Beta Pruning**: Optimized decision-making tree for the AI opponent (`PlayerMiniMax.java`).
    * **Iterative Deepening Search (IDS)**: To find the best move within a time limit (`PlayerMiniMaxIDS.java`).
    * **Heuristics**: Custom evaluation functions to calculate board advantage.

### 2. 🗺️ Search & Pathfinding Engine
* **Folder**: `Search-Algorithms-Pathfinding`
* **Description**: A generic search framework applied to topological maps.
* **Algorithms Implemented**:
    * **Uninformed Search**: Breadth-First Search (**BFS**) and Depth-First Search (**DFS**).
    * **Informed Search**: **A* (A-Star)** using Euclidean and Manhattan heuristics for optimal pathfinding.
    * **Memory Optimization**: Recursive implementations vs Iterative approaches using **Frontera** (Frontier) data structures.

### 3. 🔴 Connect-4 AI Agent
* **Folder**: `AI-Game-Strategy-Connect4`
* **Focus**: Adversarial search in zero-sum games.
* **Tech**: Implementation of a heuristic evaluation function to predict opponent moves 4 steps ahead.

---
*Tech Stack: Java 17, Maven, Swing (GUI), JUnit (Testing).*