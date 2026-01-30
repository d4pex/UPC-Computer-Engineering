// routes/serveisRoutes.js
const express = require('express');
const router = express.Router();

const serveisController = require('../controllers/serveisController');
const authMiddleware = require("../middleware/authMiddleware");

router.get('/', serveisController.getServeis);
router.post('/crear', authMiddleware, serveisController.crearServei);
router.put('/modificar/:id', authMiddleware, serveisController.modificarServei);

module.exports = router;