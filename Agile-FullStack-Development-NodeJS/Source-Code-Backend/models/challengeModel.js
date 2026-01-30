const pool = require("./db");

const challengeEnrollmentModel = {
  /**
   * Inscriu un usuari a un repte
   * @param {number} userId - ID de l'usuari
   * @param {number} repteId - ID del repte
   * @returns {Promise<boolean>} - Cert si la inscripció ha tingut èxit
   */
  async enroll(userId, repteId) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `
        INSERT INTO inscripcions_repte (usuari_id, repte_id, data_inscripcio, completat)
        VALUES (?, ?, NOW(), false)
      `;
      await conn.query(query, [userId, repteId]);
      return true;
    } catch (error) {
      console.error("Error inscrivint usuari al repte:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Comprova si un usuari ja està inscrit a un repte
   * @param {number} userId - ID de l'usuari
   * @param {number} repteId - ID del repte
   * @returns {Promise<boolean>} - Cert si ja està inscrit
   */
  async isEnrolled(userId, repteId) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `
        SELECT * FROM inscripcions_repte
        WHERE usuari_id = ? AND repte_id = ?
      `;
      const result = await conn.query(query, [userId, repteId]);
      return result.length > 0;
    } catch (error) {
      console.error("Error comprovant inscripció al repte:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Marca un repte com a completat per part de l’usuari
   * @param {number} userId - ID de l'usuari
   * @param {number} repteId - ID del repte
   * @returns {Promise<boolean>} - Cert si s’ha actualitzat correctament
   */
  async markAsCompleted(userId, repteId) {
    let conn;
    try {
      conn = await pool.getConnection();
      // Marcar como completado
      const query = `
        UPDATE inscripcions_repte
        SET completat = true
        WHERE usuari_id = ? AND repte_id = ?
      `;
      const result = await conn.query(query, [userId, repteId]);

      if (result.affectedRows > 0) {
        // Asignar badge al usuario (el badge es el id del reto)
        await conn.query(
          `INSERT IGNORE INTO usuari_badges (usuari_id, badge_id) VALUES (?, ?)`,
          [userId, repteId],
        );

        // Obtener créditos de recompensa del reto
        const recompensaRes = await conn.query(
          `SELECT credits_recompensa FROM reptes WHERE id = ? LIMIT 1`,
          [repteId],
        );
        const creditsRecompensa =
          recompensaRes.length > 0 ? recompensaRes[0].credits_recompensa : 0;

        if (creditsRecompensa && creditsRecompensa > 0) {
          // Sumar créditos al usuario
          /*await conn.query(
            `UPDATE usuaris_subscripcio SET credits = credits + ? WHERE usuari_id = ?`,
            [creditsRecompensa, userId],
          );
          */

          // Registrar la transacción de recompensa usando el modelo Transaction
          const Transaction = require("./transactionModel");
          // Obtener el saldo final después de sumar la recompensa
          const saldoRes = await conn.query(
            `SELECT credits FROM usuaris_subscripcio WHERE usuari_id = ?`,
            [userId],
          );
          const saldo_final = saldoRes.length > 0 ? saldoRes[0].credits : null;

          await Transaction.createTransaction({
            usuari_id: userId,
            tipus: "recompensa",
            quantitat: creditsRecompensa,
            saldo_final,
            descripcio: `Recompensa per completar el repte ${repteId}`,
          });
        }
        return true;
      } else {
        return false;
      }
    } catch (error) {
      console.error("Error marcant repte com completat:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Obté tots els reptes inscrits per un usuari
   * @param {number} userId - ID de l'usuari
   * @returns {Promise<Array>} - Llista de reptes inscrits
   */
  async getUserChallenges(userId) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `
        SELECT r.*, ir.completat, ir.data_inscripcio
        FROM reptes r
        JOIN inscripcions_repte ir ON r.id = ir.repte_id
        WHERE ir.usuari_id = ?
      `;
      const result = await conn.query(query, [userId]);
      return result;
    } catch (error) {
      console.error("Error obtenint reptes de l'usuari:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Elimina la inscripció d’un usuari a un repte
   * @param {number} userId - ID de l'usuari
   * @param {number} repteId - ID del repte
   * @returns {Promise<boolean>} - Cert si s’ha eliminat correctament
   */
  async removeEnrollment(userId, repteId) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `
        DELETE FROM inscripcions_repte
        WHERE usuari_id = ? AND repte_id = ?
      `;
      const result = await conn.query(query, [userId, repteId]);
      return result.affectedRows > 0;
    } catch (error) {
      console.error("Error eliminant inscripció al repte:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },
  /**
   * Obtiene el precio de un reto por su ID
   * @param {number} repteId - ID del reto
   * @returns {Promise<number>} - Precio del reto (0 si es gratuito)
   */
  async getChallengePriceById(repteId) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `
        SELECT preu
        FROM reptes
        WHERE id = ?
      `;
      const result = await conn.query(query, [repteId]);
      if (result.length === 0) {
        throw new Error("Repte no trobat");
      }
      return result[0].preu;
    } catch (error) {
      console.error("Error obtenint preu del repte:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Crea un nou repte
   * @param {Object} data - Dades del repte (nom, descripcio, preu, etc.)
   * @returns {Promise<number>} - ID del repte creat
   */
  async createChallenge(data) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `
        INSERT INTO reptes (nom, descripcio, preu, data_fi, credits_recompensa, objectiu)
        VALUES (?, ?, ?, ?, ?, ?)
      `;
      const result = await conn.query(query, [
        data.nom,
        data.descripcio,
        data.preu,
        data.data_fi,
        data.credits_recompensa,
        data.objectiu,
      ]);
      return result.insertId;
    } catch (error) {
      console.error("Error creant repte:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  async getChallengesWithFilters(filters) {
    let conn;
    try {
      conn = await pool.getConnection();
      let query = "SELECT * FROM reptes";
      let conditions = [];
      let params = [];

      if (filters.preu_min !== undefined) {
        conditions.push("preu >= ?");
        params.push(filters.preu_min);
      }
      if (filters.preu_max !== undefined) {
        conditions.push("preu <= ?");
        params.push(filters.preu_max);
      }
      if (filters.recompensa_min !== undefined) {
        conditions.push("credits_recompensa >= ?");
        params.push(filters.recompensa_min);
      }
      if (filters.recompensa_max !== undefined) {
        conditions.push("credits_recompensa <= ?");
        params.push(filters.recompensa_max);
      }
      if (filters.data_fi !== undefined) {
        conditions.push("data_fi <= ?");
        params.push(filters.data_fi);
      }
      if (filters.estat !== undefined) {
        conditions.push("actiu = ?");
        params.push(filters.estat);
      }

      if (conditions.length > 0) {
        query += " WHERE " + conditions.join(" AND ");
      }

      const result = await conn.query(query, params);
      return result;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Actualiza el progreso de una inscripción a reto
   * @param {number} inscripcioId - ID de la inscripción en inscripcions_repte
   */
  /**
   * Si el usuario está inscrito en un reto de esa clase, suma 1 al progreso.
   * @param {number} usuari_id
   * @param {number} clase_id
   * @returns {Promise<boolean>} true si se actualizó, false si no había inscripción
   */
  async incrementarProgresPerClasse(usuari_id, clase_id) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `
         SELECT ir.id, ir.progress, r.objectiu, ir.repte_id
         FROM inscripcions_repte ir
         JOIN reptes r ON ir.repte_id = r.id
         WHERE ir.usuari_id = ? AND r.clase_id = ?
         LIMIT 1
       `;
      const result = await conn.query(query, [usuari_id, clase_id]);
      if (result.length === 0) return false;

      const inscripcioId = result[0].id;
      const progressActual = result[0].progress + 1; // porque lo vamos a incrementar
      const objectiu = result[0].objectiu;
      const repte_id = result[0].repte_id;

      // Incrementar el progreso
      await conn.query(
        `UPDATE inscripcions_repte SET progress = ? WHERE id = ?`,
        [progressActual, inscripcioId],
      );

      // Si llega al objetivo, marcar como completado, asignar badge y dar créditos de recompensa
      if (progressActual >= objectiu) {
        await conn.query(
          `UPDATE inscripcions_repte SET completat = 1 WHERE id = ?`,
          [inscripcioId],
        );
        // Asignar badge (el badge es el id del reto)
        await conn.query(
          `INSERT INTO usuari_badges (usuari_id, badge_id) VALUES (?, ?)`,
          [usuari_id, repte_id],
        );

        // Obtener créditos de recompensa del reto
        const recompensaRes = await conn.query(
          `SELECT credits_recompensa FROM reptes WHERE id = ? LIMIT 1`,
          [repte_id],
        );
        const creditsRecompensa =
          recompensaRes.length > 0 ? recompensaRes[0].credits_recompensa : 0;

        if (creditsRecompensa && creditsRecompensa > 0) {
          // Sumar créditos al usuario
          await conn.query(
            `UPDATE usuaris_subscripcio SET credits = credits + ? WHERE usuari_id = ?`,
            [creditsRecompensa, usuari_id],
          );

          // Registrar la transacción de recompensa usando el modelo Transaction
          const Transaction = require("./transactionModel");
          // Obtener el saldo final después de sumar la recompensa
          const saldoRes = await conn.query(
            `SELECT credits FROM usuaris_subscripcio WHERE usuari_id = ?`,
            [usuari_id],
          );
          const saldo_final = saldoRes.length > 0 ? saldoRes[0].credits : null;

          await Transaction.createTransaction({
            usuari_id,
            tipus: "recompensa",
            quantitat: creditsRecompensa,
            saldo_final,
            descripcio: `Recompensa per completar el repte ${repte_id}`,
          });
        }
      }

      return true;
    } finally {
      if (conn) conn.release();
    }
  },
  /**
   * Obtiene el progreso de un reto para un usuario
   * @param {number} usuari_id
   * @param {number} repte_id
   * @returns {Promise<number|null>} - Progreso o null si no está inscrito
   */
  async getProgress(usuari_id, repte_id) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `
         SELECT ir.progress, r.objectiu
         FROM inscripcions_repte ir
         JOIN reptes r ON ir.repte_id = r.id
         WHERE ir.usuari_id = ? AND ir.repte_id = ?
         LIMIT 1
       `;
      const result = await conn.query(query, [usuari_id, repte_id]);
      if (result.length === 0) return null;
      return { progress: result[0].progress, objectiu: result[0].objectiu };
    } finally {
      if (conn) conn.release();
    }
  },

  // Añadir badge a un usuario
  async afegirBadgeAUsuari(usuari_id, badge_id) {
    let conn;
    try {
      conn = await pool.getConnection();
      await conn.query(
        `INSERT INTO usuari_badges (usuari_id, repte_id) VALUES (?, ?)`,
        [usuari_id, badge_id],
      );
    } finally {
      if (conn) conn.release();
    }
  },
  // Obtiene los badges (retos completados) de un usuario
  async getUserBadges(usuari_id) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `
        SELECT b.*
        FROM usuari_badges ub
        JOIN reptes b ON ub.badge_id = b.id
        WHERE ub.usuari_id = ?
      `;
      const result = await conn.query(query, [usuari_id]);
      return result;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Devuelve el id del reto (badge) asociado a una inscripción
   * @param {number} inscripcioId
   * @returns {Promise<number|null>}
   */
  async getRepteIdByInscripcioId(inscripcioId) {
    let conn;
    try {
      conn = await pool.getConnection();
      const result = await conn.query(
        `SELECT repte_id FROM inscripcions_repte WHERE id = ? LIMIT 1`,
        [inscripcioId],
      );
      return result.length > 0 ? result[0].repte_id : null;
    } finally {
      if (conn) conn.release();
    }
  },
};

module.exports = challengeEnrollmentModel;
