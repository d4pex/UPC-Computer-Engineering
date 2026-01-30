const express = require("express");
const router = express.Router();
const challengeController = require("../controllers/challengeController");

// GET /api/ranking/credits
router.get("/", challengeController.getRankingUsuariosMasCreditos);

module.exports = router;
