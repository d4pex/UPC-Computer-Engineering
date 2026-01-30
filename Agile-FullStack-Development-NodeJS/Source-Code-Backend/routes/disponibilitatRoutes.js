const express = require('express');
const router = express.Router();

const { obtenirDisponibilitat } = require('../controllers/disponibilitatController');

// Ruta: GET /api/disponibilitat?data=YYYY-MM-DD
router.get('/', obtenirDisponibilitat);

module.exports = router;