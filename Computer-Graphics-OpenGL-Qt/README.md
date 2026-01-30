# Computer Graphics & 3D Engines (OpenGL/Qt) 🚀

## 🎨 Module Overview
Practical implementation of the **3D Rendering Pipeline** from scratch using **C++**, the **Qt Framework**, and **Core OpenGL (3.3+)**. Unlike using game engines (Unity/Unreal), this module focuses on the low-level mathematics and logic behind rendering: Matrix transformations, GLSL Shader programming, and the event loop.

**Red Team & Game Hacking Relevance:**
* **Game Hacking Fundamentals:** Understanding the **Model-View-Projection (MVP)** matrix multiplication is the physics behind **Aimbots** (World-to-Screen transformation).
* **Reverse Engineering:** Knowledge of the graphics pipeline (Vertex vs Fragment shaders) and buffers (VBO/VAO) is essential for hooking graphics drivers (DirectX/OpenGL hooks) to create overlays or **Wallhacks** (disabling Z-Buffer testing).

## 📂 Projects & Labs

### 1. 🧪 The Rendering Pipeline (Labs)
* **Folders**: `Lab-1-Basic-Pipeline`, `Lab-2-Model-Loading-Cameras`, `Lab-3-Lighting-Materials`.
* **Tech**: Vertex/Fragment Shaders (GLSL), `.obj` loading.
* **Skills**:
    * **Pipeline Setup**: Configuring the OpenGL context within a Qt Widget (`MyGLWidget`).
    * **Camera Control**: Implementing Euler angles and LookAt matrices to navigate the 3D world.
    * **Lighting Models**: Implementing **Phong Shading** (Ambient + Diffuse + Specular) manually in GLSL.

### 2. 👻 Interactive Scenarios (Exams)
* **Folders**: `Exam-Coin-Interaction-Session`, `Exam-Ghost-Animation-Session`.
* **Focus**: User Interaction & Animation.
* **Skills**:
    * **Picking/Interaction**: Detecting mouse clicks on 3D objects (Ray casting logic) to collect coins.
    * **Animation**: Using timer events (`QTimer`) to update transformation matrices over time (e.g., oscillating ghost movement).

### 3. 🛸 Final Project: "Rick & Morty Scene"
* **Folder**: `Project-Rick-And-Morty-Scene`
* **Description**: A complete 3D scene composition featuring complex models and lighting interactions.
* **Features**:
    * **Multiple Shaders**: Switching shader programs at runtime for different effects.
    * **Scene Graph**: Managing hierarchical transformations (e.g., a WatchTower with rotating parts).
    * **Custom Assets**: Integration of external `.obj` and `.mtl` assets (Rick, Morty, Towers).

---
*Tech Stack: C++, Qt 5/6, OpenGL 3.3 Core, GLSL (Shading Language), Linear Algebra.*
