const db = require("../models/db");

const serveisModel = {
    /**
   * Registra un nou servei
   * @param {Object} data - Dades del servei
   * @returns {Promise<number>} - ID del servei creat
   */
  async crearServei(data) {
    let conn;
    try {
      conn = await db.getConnection();
      await conn.beginTransaction();

      const insertServeiQuery = `
        INSERT INTO serveis (ofertant_id, nom, descripcio, data, ubicacio, preu, duracio_mins, capacitat, estat_id, tipus_servei_id)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)`;

      const serveiResult = await conn.query(insertServeiQuery, [
        data.ofertant_id,
        data.nom,
        data.descripcio,
        data.data,
        data.ubicacio,
        data.preu,
        data.duracio_mins,
        data.capacitat,
        data.estat_id,
        data.tipus_servei_id
      ]);

      await conn.commit();
      return serveiResult;

    } catch (error) {
      if (conn) await conn.rollback();
      console.error("Error registrant servei:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Obté una llista de serveis segons els filtres i paràmetres de paginació
   * @param {Object} queryParams - Paràmetres de consulta
   * @returns {Promise<Array>} - Llista de serveis
   */
  async getAllServeis(queryParams) {
    let conn;
    try {
      conn = await db.getConnection();

      const {
        page = 1,
        limit = 10,
        nom,
        descripcio,
        data,
        ubicacio,
        tipus_servei_id,
        estat_id,
        preu_min,
        preu_max,
        order_by = "created_at",
        order_direction = "desc",
      } = queryParams;

      const offset = (page - 1) * limit;
      const filters = [];
      const params = [];

      if (nom) {
        filters.push(`nom LIKE ?`);
        params.push(`%${nom}%`);
      }
      if (tipus_servei_id) {
        filters.push(`tipus_servei_id = ?`);
        params.push(tipus_servei_id);
      }
      if (estat_id) {
        filters.push(`estat_id = ?`);
        params.push(estat_id);
      }
      if (preu_min) {
        filters.push(`preu >= ?`);
        params.push(preu_min);
      }
      if (preu_max) {
        filters.push(`preu <= ?`);
        params.push(preu_max);
      }

      const whereClause = filters.length ? `WHERE ${filters.join(" AND ")}` : "";
      const sql = `
        SELECT * FROM serveis
        ${whereClause}
        ORDER BY ${db.escapeId(order_by)} ${order_direction.toUpperCase() === "ASC" ? "ASC" : "DESC"}
        LIMIT ? OFFSET ?
      `;

      params.push(Number(limit), Number(offset));

      return await conn.query(sql, params);

    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Actualitza un servei existent
   * @param {number} serveiId - ID del servei
   * @param {number} ofertant_id - ID del professional
   * @param {Object} data - Camps a modificar
   * @returns {Promise<number>} - Nombre de files afectades
   */
  async actualitzarServei(serveiId, ofertant_id, data) {
    let conn;
    try {
      conn = await db.getConnection();
      await conn.beginTransaction();

      const fieldsToUpdate = [];
      const values = [];

      if (data.nom) {
        values.push(data.nom);
        fieldsToUpdate.push("nom = ?");
      }
      if (data.descripcio) {
        values.push(data.descripcio);
        fieldsToUpdate.push("descripcio = ?");
      }
      if (data.data) {
        values.push(data.data);
        fieldsToUpdate.push("data = ?");
      }
      if (data.ubicacio) {
        values.push(data.ubicacio);
        fieldsToUpdate.push("ubicacio = ?");
      }
      if (data.preu) {
        values.push(data.preu);
        fieldsToUpdate.push("preu = ?");
      }
      if (data.duracio_mins) {
        values.push(data.duracio_mins);
        fieldsToUpdate.push("duracio_mins = ?");
      }
      if (data.capacitat) {
        values.push(data.capacitat);
        fieldsToUpdate.push("capacitat = ?");
      }
      if (data.estat_id) {
        values.push(data.estat_id);
        fieldsToUpdate.push("estat_id = ?");
      }
      if (data.tipus_servei_id) {
        values.push(data.tipus_servei_id);
        fieldsToUpdate.push("tipus_servei_id = ?");
      }

      if (fieldsToUpdate.length === 0) {
        return false;
      }

      values.push(serveiId);
      values.push(ofertant_id);

      const updateQuery = `UPDATE serveis SET ${fieldsToUpdate.join(", ")} WHERE id = ? AND ofertant_id = ?`;
      const result = await conn.query(updateQuery, values);
      await conn.commit();
      return result.affectedRows;

    } catch (error) {
      if (conn) await conn.rollback();
      console.error("Error actualitzant servei:", error);
      throw error;
    } finally {
      if (conn) conn.release();
    }
  }
};

module.exports = serveisModel;

