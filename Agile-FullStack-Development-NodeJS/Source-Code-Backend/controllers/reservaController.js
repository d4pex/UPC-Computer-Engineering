// controllers/reservaController.j
const reservaModel = require("../models/reservaModel");
const challengeModel = require("../models/challengeModel");
async function crearReserva(req, res) {
  const { usuari_id, servei_id, data_reserva } = req.body;

  if (!usuari_id || !servei_id || !data_reserva) {
    return res.status(400).json({ error: "Falten camps obligatoris" });
  }

  try {
    const resultat = await reservaModel.crearReservaDB(usuari_id, servei_id, data_reserva);
    await challengeModel.incrementarProgresPerClasse(usuari_id, servei_id);
    res.status(201).json(resultat);
  } catch (error) {
    console.error("Error creant reserva:", error);
    res.status(500).json({ error: "Error intern del servidor" });
  }
}

async function veureReservesProfesional(req, res) {
    const { userId: idx } = req.user || {};

  if (!idx) {
    return res.status(401).json({ error: "Usuari no autenticat" });
  }

    try {
        const reserves = await reservaModel.obtenirReservesPerProfessional(idx);
        res.status(200).json(reserves);
    } catch(error) {
        console.error("Error obtenint reserves:", error);
        res.status(500).json({ error: "Error intern del servidor" });
    }
}

async function veureReservesUsuari(req, res) {
  const { userId: idx } = req.user || {};

  if(!idx) {
    return res.status(401).json({ error: "Usuari no autenticat" });
  }

  try {
      const reserves = await reservaModel.obtenirReservesPerUsuari(idx);
      res.status(200).json(reserves);
  } catch(error) {
      console.error("Error obtenint reserves:", error);
      res.status(500).json({ error: "Error intern del servidor" });
  }
}

module.exports = { crearReserva, veureReservesProfesional, veureReservesUsuari };
