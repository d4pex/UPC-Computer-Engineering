const { obtenirServeisPerCategoria } = require('../models/serveisFiltratsModel');

/**
 * Controlador que retorna serveis filtrats per categoria si s'especifica.
 * Ex: GET /api/serveis/filtrats?categoria=Salut
 */
async function obtenirServeisFiltrats(req, res) {
  try {
    const categoria = req.query.categoria || null;

    const serveis = await obtenirServeisPerCategoria(categoria);

    res.status(200).json(serveis);
  } catch (error) {
    console.error('Error obtenint serveis filtrats:', error);
    res.status(500).json({ missatge: 'Error intern en obtenir els serveis filtrats.' });
  }
}

module.exports = {
  obtenirServeisFiltrats
};