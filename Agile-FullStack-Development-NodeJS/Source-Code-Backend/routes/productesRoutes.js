const express = require('express');
const router = express.Router();
const { veureProductes } = require('../controllers/productesController');

router.post('/', veureProductes);

module.exports = router;
