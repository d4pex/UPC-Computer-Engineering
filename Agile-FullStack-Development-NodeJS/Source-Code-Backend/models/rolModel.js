const pool = require("./db");

const roleModel = {
  /**
   * Obté l'ID d'un rol pel seu nom
   * @param {string} rolNom - Nom del rol
   * @returns {Promise<number|null>} - ID del rol o null
   */
  async getIdByName(rolNom) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `SELECT id FROM rols WHERE nom = ?`;
      const result = await conn.query(query, [rolNom]);
      return result.length > 0 ? result[0].id : null;
    } catch (error) {
      console.error("Error obtenint rol:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },
};

module.exports = roleModel;
