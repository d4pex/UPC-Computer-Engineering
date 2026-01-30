const express = require("express");
const router = express.Router();
const subscripcionsController = require("../controllers/subscripcionsController");

router.put("/seleccionar", subscripcionsController.seleccionarSubscripcio);

module.exports = router;
