# Enterprise Software Engineering: "PetitFlix" 🎬

## 🏗️ Module Overview
Full-cycle development of a video streaming platform management system ("PetitFlix"). This project covers the entire **Software Development Life Cycle (SDLC)**: from requirements engineering and UML modeling to the implementation of a **3-Tier Architecture** in **C++** with **MySQL** persistence.

**Red Team & Security Relevance:**
* **Business Logic Analysis:** Understanding how Enterprise applications manage sessions (`TxIniciSessio`), transactions, and state is critical for identifying **Logic Vulnerabilities** (IDORs, Privilege Escalation).
* **Database Interactions:** Designing the Data Access Layer (`ConnexioBD`, `Finders`) provides insight into how **SQL Injection** flaws occur when inputs are not sanitized in the backend.
* **Secure Architecture:** The separation of concerns (Presentation vs Domain vs Data) is the foundational concept of **Defense in Depth**.

## 📂 Project Structure & Architecture

The system follows a strict **Three-Tier Architecture** to decouple user interaction, business logic, and data storage.

### 1. 🖥️ Presentation Layer
* **Folder**: `Source-Code-PetitFlix/Presentation-Layer`
* **Role**: Handles user input and displays data.
* **Components**:
    * `menu.cpp`: Main entry point and event loop.
    * `CapaDePresentacio`: Interface that captures user commands and calls the Domain Layer.

### 2. 🧠 Domain Layer (Business Logic)
* **Folder**: `Source-Code-PetitFlix/Domain-Layer`
* **Pattern**: **Transaction Script** & **Domain Controllers**.
* **Key Components**:
    * **Transactions (`Tx*`)**: Atomic business operations.
        * `TxIniciSessio`: Handles authentication and session tokens.
        * `TxRegistraUsuari`: User creation logic and validation.
        * `TxConsultarVisualitzacions`: Retrieval of user history.
    * **Controllers (`Ctrl*`)**: Orchestrate complex flows like `CtrlVisualitzaPelicula`.
    * **Gateways (`Passarela*`)**: Objects that represent a row in the database table (Active Record / Row Data Gateway pattern).

### 3. 💾 Data Persistence Layer
* **Folder**: `Source-Code-PetitFlix/Data-Layer`
* **Pattern**: **Table Data Gateway / Finders**.
* **Role**: Abstracts the SQL logic from the rest of the application.
* **Key Components**:
    * **Finders (`Finders/` / `Cercadora*`)**: Classes dedicated to executing `SELECT` queries (e.g., `UserFinder` finds a user by Nickname).
    * **Connection**: `ConnexioBD` manages the JDBC/MySQL driver and connection pool.

### 4. 📦 Data Transfer Objects (DTOs)
* **Folder**: `Source-Code-PetitFlix/DTOs`
* **Role**: Lightweight objects used to transport data between layers without exposing the internal domain entities.

---

## 📄 Documentation & SDLC Phases

* **Folder**: `Documentation/`

1.  **Phase 1: Requirements Specification** (`Phase-1-Requirements.pdf`)
    * Functional requirements elicitation.
    * **UML Use Case** diagrams defining system actors and boundaries.
2.  **Phase 2: System Design** (`Phase-2-Architecture.pdf`)
    * **UML Sequence Diagrams** modeling runtime interactions.
    * Class diagrams defining the static view and relationships.
3.  **Phase 3: Implementation** (`Phase-3-Implementation.pdf`)
    * Details on the C++ implementation, SQL schema design, and testing strategies.

---
*Tech Stack: C++, MySQL (MariaDB), UML (Enterprise Architect), Makefile.*