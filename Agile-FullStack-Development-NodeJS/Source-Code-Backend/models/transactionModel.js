const pool = require("./db");

const Transaction = {
  // Crear una nova transacció
  async createTransaction({
    usuari_id,
    tipus,
    quantitat,
    saldo_final,
    pla_subscripcio_id = null,
    preu = null,
    descripcio = null,
  }) {
    let conn;
    try {
      conn = await pool.getConnection();
      const sql = `
        INSERT INTO transaccions
        (usuari_id, tipus, quantitat, saldo_final, pla_subscripcio_id, preu, descripcio)
        VALUES (?, ?, ?, ?, ?, ?, ?)
      `;
      const result = await conn.query(sql, [
        usuari_id,
        tipus,
        quantitat,
        saldo_final,
        pla_subscripcio_id,
        preu,
        descripcio,
      ]);
      return {
        usuari_id,
        tipus,
        quantitat,
        saldo_final,
        pla_subscripcio_id,
        preu,
        descripcio,
      };
    } catch (error) {
      console.error("Error creant transacció:", error);
      throw error;
    } finally {
      if (conn) {
        conn.release();
      }
    }
  },

  // Obtenir totes les transaccions d’un usuari
  async getTransactionsByUser(usuari_id) {
    let conn;
    try {
      conn = await pool.getConnection();
      const sql = `SELECT * FROM transaccions WHERE usuari_id = ? ORDER BY data_transaccio DESC`;
      const rows = await conn.query(sql, [usuari_id]);
      return rows;
    } catch (error) {
      console.error("Error obtenint transaccions per usuari:", error);
      throw error;
    } finally {
      if (conn) {
        conn.release();
      }
    }
  },

  // Obtenir el saldo actual d’un usuari (últim saldo_final)
  async getLatestBalance(usuari_id) {
    let conn;
    try {
      conn = await pool.getConnection();
      const sql = `
        SELECT saldo_final FROM transaccions
        WHERE usuari_id = ?
        ORDER BY data_transaccio DESC
        LIMIT 1
      `;
      const rows = await conn.query(sql, [usuari_id]);
      return rows.length > 0 ? rows[0].saldo_final : 0;
    } catch (error) {
      console.error("Error obtenint el saldo actual:", error);
      throw error;
    } finally {
      if (conn) {
        conn.release();
      }
    }
  },
};

module.exports = Transaction;
