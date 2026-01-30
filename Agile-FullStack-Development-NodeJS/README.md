# Agile Full-Stack Development: "Mou-te+" 🏃‍♂️

## 🚀 Module Overview
Development of a complete web platform ("Mou-te+") using **Agile Methodologies (Scrum)** and a modern **Full-Stack JavaScript** architecture. The project simulates a real-world startup environment, covering everything from Product Inception and User Stories to the deployment of a REST API with Node.js.

**Security & Red Team Relevance:**
* **API Security:** Building a REST API (`routes/`, `controllers/`) provides deep insight into common vulnerabilities like **Broken Object Level Authorization (BOLA)** and **Mass Assignment**.
* **Authentication Logic:** Implementation of `authMiddleware.js` and session handling is critical for understanding attacks on **Identity Management** (Session Hijacking, JWT attacks).
* **Modern Stack Analysis:** Proficiency in **Node.js/Express** is essential for auditing modern web applications (AppSec).

## 📂 Project Components

### 1. ⚙️ Backend (Node.js & Express)
* **Folder**: `Source-Code-Backend`
* **Architecture**: MVC (Model-View-Controller) adapted for API services.
* **Key Security Features**:
    * **Middleware**: Custom authentication logic (`middleware/authMiddleware.js`) to protect endpoints.
    * **Controllers**: Logic for critical operations like `paymentController.js` and `authController.js`.
    * **Data Models**: Mongoose/Sequelize schemas defining data structure and validation rules.

### 2. 🎨 Frontend (HTML5/JS)
* **Folder**: `Source-Code-Frontend`
* **Tech**: Native JavaScript (ES6+), HTML5, CSS3.
* **Features**:
    * Dynamic DOM manipulation (`js/dashboard.js`, `js/login.js`).
    * Async communication with the Backend API using `fetch`/`axios`.

### 3. 📅 Agile Project Management
* **Folder**: `Project-Management-Agile`
* **Methodology**: **Scrum** (3 Sprints).
* **Deliverables**:
    * **Inception Phase**: Market study, Product Box, and initial Backlog (`00-Inception-Report.pdf`).
    * **Sprint Planning & Review**: Detailed breakdown of User Stories, Story Points estimation, and Burndown Charts for each iteration.
    * **Retrospectives**: Analysis of team velocity and process improvements.

---
*Tech Stack: Node.js, Express, JavaScript, HTML/CSS, Scrum, Git.*