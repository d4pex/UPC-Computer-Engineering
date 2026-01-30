// controllers/cancelReservaController.js
const { cancelarReservaDB } = require("../models/cancelReservaModel");

async function cancelarReserva(req, res) {
  const reservaId = req.params.id;
  const { usuari_id } = req.body;

  try {
    const resposta = await cancelarReservaDB(reservaId, usuari_id);
    res.status(200).json(resposta);
  } catch (error) {
    console.error("Error cancel·lant reserva:", error);
    res
      .status(error.status || 500)
      .json({ error: error.message || "Error intern del servidor" });
  }
}

module.exports = { cancelarReserva };
