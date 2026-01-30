// routes/cancelReservaRoutes.js
const express = require('express');
const router = express.Router();

const { cancelarReserva } = require('../controllers/cancelReservaController');

router.put('/:id/cancelar', cancelarReserva);

module.exports = router;