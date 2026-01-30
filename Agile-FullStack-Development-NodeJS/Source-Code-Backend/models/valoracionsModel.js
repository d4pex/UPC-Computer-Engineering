// models/valoracionsModel.js
const pool = require("./db");

const valoracionsModel = {
/**
 * Obté les valoracions dels serveis d’un professional
 * @param {number} userId - ID del professional (ofertant)
 * @returns {Promise<Array>} - Llista de valoracions
 */
async obtenirValoracionsPerProfessional(userId) {
    let conn;
    try {
      conn = await pool.getConnection();

      const query = `
        SELECT v.*, u.nom AS usuari_nom, s.nom AS servei_nom
        FROM valoracions v
        JOIN serveis s ON v.servei_id = s.id
        JOIN usuaris u ON v.usuari_id = u.id
        WHERE s.ofertant_id = ?
        ORDER BY v.created_at DESC
      `;

      const result = await conn.query(query, [userId]);
      return result;
    } catch (error) {
      console.error("Error obtenint valoracions:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Crea una nova valoració
   * @param {Object} data - Dades de la valoració
   * @returns {Promise<Object>} - Resultat de la inserció
   */
  async crearValoracio(data) {
    let conn;
    try {
      conn = await pool.getConnection();
      await conn.beginTransaction();

      const existeix = await conn.query(
        `SELECT id FROM valoracions WHERE usuari_id = ? AND servei_id = ?`,
        [data.usuari_id, data.servei_id]
      );

      if (existeix.length > 0) {
        await conn.rollback();
        return { existeix: true };
      }

      const insert = await conn.query(
        `INSERT INTO valoracions (usuari_id, servei_id, puntuacio, comentari)
         VALUES (?, ?, ?, ?)`,
        [data.usuari_id, data.servei_id, data.puntuacio, data.comentari]
      );

      await conn.commit();
      return { existeix: false, insert };
    } catch (error) {
      if (conn) await conn.rollback();
      console.error("Error creant valoració:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  }
};

module.exports = valoracionsModel;
