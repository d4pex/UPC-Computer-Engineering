const pool = require("./db");
const bcrypt = require("bcrypt");

const userModel = {
  /**
   * Comprova si existeix un usuari amb el mateix DNI o email
   * @param {string} dni - DNI de l'usuari
   * @param {string} mail - Correu electrònic
   * @returns {Promise<boolean>} - Cert si l'usuari existeix
   */
  async exists(dni, mail) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `SELECT * FROM usuaris WHERE dni = ? OR mail = ?`;
      const result = await conn.query(query, [dni, mail]);
      return result.length > 0;
    } catch (error) {
      console.error("Error comprovant existència d'usuari:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Registra un nou usuari
   * @param {Object} userData - Dades de l'usuari
   * @returns {Promise<number>} - ID de l'usuari creat
   */
  async register(userData) {
    let conn;
    try {
      conn = await pool.getConnection();
      await conn.beginTransaction();

      // Hash de la contrasenya
      const hashedPassword = await bcrypt.hash(userData.contrasenya, 10);

      // Inserir usuari
      const insertUserQuery = `
        INSERT INTO usuaris (dni, nom, sobrenom, contrasenya, mail, telefon, rol_id)
        VALUES (?, ?, ?, ?, ?, ?, ?)
      `;
      const userResult = await conn.query(insertUserQuery, [
        userData.dni,
        userData.nom,
        userData.sobrenom,
        hashedPassword,
        userData.mail,
        userData.telefon,
        userData.rolId,
      ]);

      const userId = userResult.insertId;

      // Inserir segons el rol
      if (userData.rol === "Usuari") {
        await conn.query(
          `INSERT INTO usuaris_subscripcio (usuari_id, subscripcio_id, credits, reptes_complets)
           VALUES (?, 1, 0, 0)`,
          [userId],
        );
      } 
      await conn.commit();
      return userId;
    } catch (error) {
      if (conn) await conn.rollback();
      console.error("Error registrant usuari:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Busca un usuari pel seu DNI
   * @param {string} dni - DNI de l'usuari
   * @returns {Promise<Object|null>} - Dades de l'usuari o null
   */
  async findByDni(dni) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `SELECT * FROM usuaris WHERE dni = ?`;
      const result = await conn.query(query, [dni]);
      return result.length > 0 ? result[0] : null;
    } catch (error) {
      console.error("Error cercant usuari per DNI:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Obté el hash de la contrasenya d'un usuari pel seu ID
   * @param {number} userId - ID de l'usuari
   * @returns {Promise<string|null>} - Hash de la contrasenya o null
   */
  async getPasswordHash(userId) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = "SELECT contrasenya FROM usuaris WHERE id = ?";
      const result = await conn.query(query, [userId]);

      if (!result || result.length === 0) {
        return null;
      }

      return result[0].contrasenya;
    } catch (error) {
      console.error("Error obtenint hash de contrasenya:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Elimina un usuari pel seu ID
   * @param {number} userId - ID de l'usuari
   * @returns {Promise<boolean>} - Cert si s'ha eliminat correctament
   */
  async deleteUser(userId) {
    let conn;
    try {
      conn = await pool.getConnection();
      await conn.beginTransaction();

      // Eliminar l'usuari
      // Nota: aquesta operació hauria d'activar eliminacions en cascada a la base de dades
      // o hauríem d'afegir més consultes per eliminar registres relacionats
      await conn.query("DELETE FROM usuaris WHERE id = ?", [userId]);

      await conn.commit();
      return true;
    } catch (error) {
      if (conn) await conn.rollback();
      console.error("Error eliminant usuari:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },
  /**
   * Obté tota la informació d'un usuari pel seu ID
   * @param {number} userId - ID de l'usuari
   * @returns {Promise<Object|null>} - Dades de l'usuari o null
   */
  async findById(userId) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = "SELECT * FROM usuaris WHERE id = ?";
      const rows = await conn.query(query, [userId]);
      return rows.length > 0 ? rows[0] : null;
    } catch (error) {
      console.error("Error obtenint usuari per ID:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Comprova si un correu electrònic ja està en ús (excloent l'usuari actual)
   * @param {string} email - Correu electrònic a comprovar
   * @param {number} excludeUserId - ID de l'usuari a excloure de la comprovació
   * @returns {Promise<boolean>} - Cert si el correu ja està en ús
   */
  async checkEmailExists(email, excludeUserId = null) {
    let conn;
    try {
      conn = await pool.getConnection();
      let query = "SELECT id FROM usuaris WHERE mail = ?";
      let params = [email];

      if (excludeUserId) {
        query += " AND id != ?";
        params.push(excludeUserId);
      }

      const rows = await conn.query(query, params);
      return rows.length > 0;
    } catch (error) {
      console.error("Error comprovant correu electrònic:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Actualitza el perfil d'un usuari
   * @param {number} userId - ID de l'usuari
   * @param {Object} userData - Dades a actualitzar
   * @returns {Promise<boolean>} - Cert si s'ha actualitzat correctament
   */
  async updateProfile(userId, userData) {
    let conn;
    try {
      conn = await pool.getConnection();

      let fieldsToUpdate = [];
      let values = [];

      // Construir la consulta dinàmicament basada en els camps proporcionats
      if (userData.sobrenom) {
        fieldsToUpdate.push("sobrenom = ?");
        values.push(userData.sobrenom);
      }
      if (userData.nom) {
        fieldsToUpdate.push("nom = ?");
        values.push(userData.nom);
      }
      if (userData.telefon) {
        fieldsToUpdate.push("telefon = ?");
        values.push(userData.telefon);
      }
      if (userData.mail) {
        fieldsToUpdate.push("mail = ?");
        values.push(userData.mail);
      }

      if (fieldsToUpdate.length === 0) {
        return false; // No hi ha camps per actualitzar
      }

      values.push(userId);

      const query = `UPDATE usuaris SET ${fieldsToUpdate.join(", ")} WHERE id = ?`;
      const result = await conn.query(query, values);

      return result && result.affectedRows > 0;
    } catch (error) {
      console.error("Error actualitzant perfil d'usuari:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Canvia la contrasenya d'un usuari
   * @param {number} userId - ID de l'usuari
   * @param {string} newPasswordHash - Nova contrasenya ja hashejada
   * @returns {Promise<boolean>} - Cert si s'ha canviat correctament
   */
  async changePassword(userId, newPasswordHash) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = "UPDATE usuaris SET contrasenya = ? WHERE id = ?";
      const result = await conn.query(query, [newPasswordHash, userId]);
      return result && result.affectedRows > 0;
    } catch (error) {
      console.error("Error canviant contrasenya:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  async getCurrentCredits(userId) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query =
        "SELECT credits FROM usuaris_subscripcio WHERE usuari_id = ?";
      const rows = await conn.query(query, [userId]);
      return rows[0].credits;
    } catch (error) {
      console.error("Error obtenint credits actuals:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Resta crèdits a un usuari
   * @param {number} userId - ID de l'usuari
   * @param {number} amount - Quantitat de crèdits a restar
   * @returns {Promise<boolean>} - Cert si s'ha actualitzat correctament
   */
  async decrementCredits(userId, amount) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `
        UPDATE usuaris_subscripcio
        SET credits = credits - ?
        WHERE usuari_id = ? AND credits >= ?
      `;
      const result = await conn.query(query, [amount, userId, amount]);
      return result.affectedRows > 0;
    } catch (error) {
      console.error("Error restant crèdits a l'usuari:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },
  /**
   * Obtiene el ranking de usuarios con más créditos
   * @param {number} [limit=10] - Número de usuarios a devolver
   * @returns {Promise<Array>} - Lista de usuarios ordenados por créditos descendente
   */
  async getRankingUsuariosMasCreditos(limit = 10) {
    let conn;
    try {
      conn = await pool.getConnection();
      const query = `
        SELECT u.id, u.nom, u.sobrenom, u.mail, us.credits
        FROM usuaris u
        JOIN usuaris_subscripcio us ON u.id = us.usuari_id
        ORDER BY us.credits DESC
        LIMIT ?
      `;
      const rows = await conn.query(query, [limit]);
      return rows;
    } catch (error) {
      console.error("Error obtenint ranking d'usuaris:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },
};


module.exports = userModel;
