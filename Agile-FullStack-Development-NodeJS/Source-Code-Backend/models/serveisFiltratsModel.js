const db = require('./db');

/**
 * Obté serveis filtrats per nom de categoria (ex: "Salut", "Esportiu"...)
 * @param {string|null} categoria Nom de la categoria o null
 * @returns {Array} Llista de serveis
 */
async function obtenirServeisPerCategoria(categoria) {
  let conn;
  try {
    conn = await db.getConnection();

    let query = `
      SELECT s.*
      FROM serveis s
      JOIN tipus_servei t ON s.tipus_servei_id = t.id
    `;
    const params = [];

    if (categoria) {
      query += ` WHERE t.nombre = ?`;
      params.push(categoria);
    }

    const serveis = await conn.query(query, params);
    return serveis;

  } finally {
    if (conn) conn.release();
  }
}

module.exports = {
  obtenirServeisPerCategoria
};