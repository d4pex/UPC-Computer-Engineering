const express = require('express');
const router = express.Router();

const { obtenirServeisFiltrats } = require('../controllers/serveisFiltratsController');

// Ruta: GET /api/serveis/filtrats?categoria=...
router.get('/filtrats', obtenirServeisFiltrats);

module.exports = router;