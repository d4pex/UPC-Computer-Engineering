// routes/userRoutes.js
const express = require("express");
const router = express.Router();
const { deleteAccount } = require("../controllers/esborraController");
const authMiddleware = require("../middleware/authMiddleware"); // middleware que valida l'autenticació

// Tasca 1 i Tasca 2: Crear l'endpoint i validar que l'usuari està autenticat
router.post("/eliminar", authMiddleware, deleteAccount);

module.exports = router;
