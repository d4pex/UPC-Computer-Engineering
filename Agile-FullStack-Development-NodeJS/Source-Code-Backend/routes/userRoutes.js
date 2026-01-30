const express = require("express");
const router = express.Router();

const {
  updateUserProfile,
  getUserById,
  changeUserPassword,
} = require("../controllers/userController");
const authenticateToken = require("../middleware/authMiddleware");

const userController = require("../controllers/userController");

const creditController = require("../controllers/creditController");

const paymentController = require("../controllers/paymentController");

const {
  createTransaction,
  getTransactionsByUser,
} = require("../controllers/transactionController");

// Ruta per consultar crèdits
router.get(
  "/usuari/credits",
  authenticateToken,
  creditController.getUserCredits,
);
router.put(
  "/usuari/canviar-contrasenya",
  authenticateToken,
  changeUserPassword,
);
router.post(
  "/usuari/pagament",
  authenticateToken,
  paymentController.processPayment,
);
// Ruta para modificar la información del usuario
router.put("/usuari/actualitzar", authenticateToken, updateUserProfile);

router.post("/usuari/transaccions", authenticateToken, createTransaction);

// Obtener todas las transacciones de un usuario
router.get("/usuari/transaccions", authenticateToken, getTransactionsByUser);
// Ruta para obtener la información del usuario por ID
router.get("/usuari/:id", userController.getUserById);
// Ruta para modificar la contraseña del usuario
module.exports = router;
