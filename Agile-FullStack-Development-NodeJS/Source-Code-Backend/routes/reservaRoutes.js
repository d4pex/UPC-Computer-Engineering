// routes/reservaRoutes.js
const express = require("express");
const router = express.Router();
const reservaController = require("../controllers/reservaController");
const authMiddleware = require("../middleware/authMiddleware");

router.post("/", reservaController.crearReserva); //Le estamos diciendo que cuando llegue una petición POST a la ruta base, se ejecute el controlador.
router.get("/visualitzar", authMiddleware, reservaController.veureReservesProfesional);
router.get("/visualitzar/jo", authMiddleware, reservaController.veureReservesUsuari);

module.exports = router;