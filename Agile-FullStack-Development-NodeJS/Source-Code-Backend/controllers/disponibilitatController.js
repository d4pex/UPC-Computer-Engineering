const { obtenirDisponibilitatPerData } = require('../models/disponibilitatModel');

/**
 * Endpoint: GET /api/disponibilitat?data=YYYY-MM-DD
 * Retorna disponibilitat per a tots els serveis en una data concreta.
 */
async function obtenirDisponibilitat(req, res) {
  try {
    const data = req.query.data;

    // Validació bàsica de la data
    if (!data || !/^\d{4}-\d{2}-\d{2}$/.test(data)) {
      return res.status(400).json({ missatge: 'Format de data invàlid. Cal format YYYY-MM-DD.' });
    }

    const disponibilitat = await obtenirDisponibilitatPerData(data);

    res.status(200).json(disponibilitat);
  } catch (error) {
    console.error('Error obtenint disponibilitat:', error);
    res.status(500).json({ missatge: 'Error intern en obtenir disponibilitat.' });
  }
}

module.exports = {
  obtenirDisponibilitat
};