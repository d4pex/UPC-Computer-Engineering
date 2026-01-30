const pool = require("./db");

const subscriptionModel = {
  /**
   * Obté les dades d'un pla de subscripció
   * @param {number} subscriptionId - ID de la subscripció
   * @returns {Promise<Object|null>} - Dades de la subscripció o null
   */
  async getSubscriptionById(subscriptionId) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = "SELECT * FROM pla_subscripcio WHERE id = ?";
      const result = await conn.query(query, [subscriptionId]);
      return result.length > 0 ? result[0] : null;
    } catch (error) {
      console.error("Error obtenint subscripció:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Obté la subscripció actual d'un usuari
   * @param {number} userId - ID de l'usuari
   * @returns {Promise<Object|null>} - Dades de la subscripció actual o null
   */
  async getUserSubscription(userId) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = "SELECT * FROM usuaris_subscripcio WHERE usuari_id = ?";
      const result = await conn.query(query, [userId]);
      return result.length > 0 ? result[0] : null;
    } catch (error) {
      console.error("Error obtenint subscripció d'usuari:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Actualitza o crea una subscripció per a un usuari
   * @param {number} userId - ID de l'usuari
   * @param {number} subscriptionId - ID del pla de subscripció
   * @param {number} credits - Quantitat de crèdits a assignar
   * @returns {Promise<boolean>} - Cert si s'ha realitzat correctament
   */
  async setUserSubscription(userId, subscriptionId, credits) {
    let conn;
    try {
      conn = await pool.getConnection();
      await conn.beginTransaction();

      // Comprovar si ja existeix una subscripció
      const currentSubscription = await this.getUserSubscription(userId);

      if (currentSubscription) {
        // Actualitzar subscripció existent
        await conn.query(
          "UPDATE usuaris_subscripcio SET subscripcio_id = ?, credits = ? WHERE usuari_id = ?",
          [subscriptionId, credits, userId],
        );
      } else {
        // Crear nova subscripció
        await conn.query(
          "INSERT INTO usuaris_subscripcio (usuari_id, subscripcio_id, credits) VALUES (?, ?, ?)",
          [userId, subscriptionId, credits],
        );
      }

      await conn.commit();
      return true;
    } catch (error) {
      if (conn) await conn.rollback();
      console.error("Error actualitzant subscripció:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },
};

module.exports = subscriptionModel;
