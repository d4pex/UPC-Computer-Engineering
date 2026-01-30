// routes/valoracionsRoutes.js
const express = require('express');
const router = express.Router();
const { veureValoracions, afegirValoracio } = require('../controllers/valoracionsController');
const authenticateToken = require('../middleware/authMiddleware');

// Veure valoracions (professional)
router.get('/', authenticateToken, veureValoracions);
// Crear valoracions (usuaris)
router.post("/crear", authenticateToken, afegirValoracio);

module.exports = router;
