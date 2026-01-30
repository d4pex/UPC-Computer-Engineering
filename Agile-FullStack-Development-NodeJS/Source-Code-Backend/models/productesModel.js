const pool = require('./db');

const productesModel = {
  /**
   * Obté productes filtrats per una o més categories
   * @param {Array<number>|null} categories - Llista d'IDs de categoria
   * @returns {Promise<Array>}
   */
  async obtenirProductes(categories = null) {
    let conn;
    try {
      conn = await pool.getConnection();

      let query = `
        SELECT 
          p.id, p.nom, p.descripcio, p.preu, p.imatge, p.url_compra,
          c.id AS categoria_id, c.nom AS categoria_nom
        FROM productes p
        INNER JOIN categories_producte c ON p.categoria_id = c.id
      `;

      const params = [];

      if (categories && categories.length > 0) {
        const placeholders = categories.map(() => '?').join(', ');
        query += ` WHERE p.categoria_id IN (${placeholders})`;
        params.push(...categories);
      }

      query += ` ORDER BY p.categoria_id ASC, p.nom ASC`;

      const result = await conn.query(query, params);
      return result;
    } catch (error) {
      console.error('Error obtenint productes:', error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  }
};

module.exports = productesModel;
