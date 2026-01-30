// controllers/valoracionsController.js
const valoracionsModel = require('../models/valoracionsModel');

const veureValoracions = async (req, res) => {
  const { userId: idx } = req.user || {};

  try {
    const valoracions = await valoracionsModel.obtenirValoracionsPerProfessional(idx);
    res.status(200).json(valoracions);
  } catch (error) {
    res.status(500).json({ message: "Error obtenint les valoracions" });
  }
};

const afegirValoracio = async (req, res) => {
  const { userId: usuari_id } = req.user || {};
  const { servei_id, puntuacio, comentari } = req.body;

  if (!usuari_id || !servei_id || !puntuacio) {
    return res.status(400).json({ message: "Falten camps obligatoris" });
  }
  if (puntuacio < 1 || puntuacio > 5) {
    return res.status(400).json({ message: "Valor de puntuació incorrecte" });
  }

  try {
    const resultat = await valoracionsModel.crearValoracio({
      usuari_id,
      servei_id,
      puntuacio,
      comentari
    });

    if (resultat.existeix) {
      return res.status(409).json({ message: "Ja has valorat aquest servei" });
    }

    res.status(201).json({ message: "Valoració enregistrada correctament" });
  } catch (error) {
    res.status(500).json({ message: "Error del servidor" });
    console.error(error);
  }
};

module.exports = { veureValoracions, afegirValoracio };
