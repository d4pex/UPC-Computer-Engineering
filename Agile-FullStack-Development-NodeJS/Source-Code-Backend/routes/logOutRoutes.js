// src/routes/userRoutes.js
const express = require("express");
const router = express.Router();

const { logoutUser } = require("../controllers/logOutController");
const authMiddleware = require("../middleware/authMiddleware");
router.post("/logout", authMiddleware, logoutUser);

module.exports = router;
