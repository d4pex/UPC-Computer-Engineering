const productesModel = require('../models/productesModel');

/**
 * Retorna productes filtrats per categories (POST amb body)
 * @route POST /api/productes
 */
const veureProductes = async (req, res) => {
  const { categories } = req.body;

  // Validació i conversió segura
  const categoriesArray = Array.isArray(categories)
    ? categories.filter(id => Number.isInteger(id))
    : null;

  try {
    const productes = await productesModel.obtenirProductes(categoriesArray);

    if (productes.length === 0) {
      return res.status(404).json({ message: 'No hi ha productes disponibles' });
    }

    res.status(200).json(productes);
  } catch (error) {
    console.error('Error veient catàleg:', error);
    res.status(500).json({ message: 'Error del servidor' });
  }
};

module.exports = { veureProductes };
