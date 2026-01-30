const db = require("../models/db");

const reservaModel = {
  /**
   * Crea una nova reserva per a un servei concret si hi ha places disponibles
   * @param {number} usuari_id - ID de l'usuari que fa la reserva
   * @param {number} servei_id - ID del servei que es vol reservar
   * @param {string} data_reserva - Data de la reserva (format YYYY-MM-DD)
   * @returns {Promise<Object>} - Objecte amb missatge i dades de la reserva creada
   * @throws {Error} - Si no hi ha places disponibles o hi ha un problema amb la consulta
   */
  async crearReservaDB(usuari_id, servei_id, data_reserva) {
    console.log("DADES rebudes:", { usuari_id, servei_id, data_reserva });

    let conn;
    try {
      conn = await db.getConnection();

      const reservesResult = await conn.query(
        `SELECT COUNT(*) as total FROM reserves WHERE servei_id = ? AND data_reserva = ? AND estat_id = 1`,
        [servei_id, data_reserva],
      );

      console.log("Reserves fetes:", reservesResult);

      if (!reservesResult || reservesResult.length === 0) {
        throw new Error("Error comptant reserves");
      }

      const reservesFetes = Number(reservesResult[0].total);

      const serveisResult = await conn.query(
        `SELECT capacitat, preu FROM serveis WHERE id = ?`,
        [servei_id],
      );

      console.log("Resultat de serveis:", serveisResult);

      if (!serveisResult || serveisResult.length === 0) {
        throw new Error("Servei no trobat");
      }

      const capacitat = serveisResult[0].capacitat;
      const preuServei = serveisResult[0].preu;

      // Por defecto, reserva disponible
      let estat_id = 1;
      let missatge = "Reserva creada correctament";

      if (reservesFetes >= capacitat) {
        throw new Error("No hi ha places disponibles");
      }

      // Si justo con esta reserva se llena la capacidad, actualiza estat_id de serveis a 2
      if (reservesFetes + 1 === capacitat) {
        await conn.query(
          `UPDATE serveis SET estat_id = 2 WHERE id = ?`,
          [servei_id]
        );
        missatge = "Reserva creada: s'ha assolit la capacitat màxima amb aquesta reserva (estat_id de servei = 2)";
      }

      // Comprobar créditos actuales del usuario antes de reservar
      const userResult = await conn.query(
        `SELECT credits FROM usuaris_subscripcio WHERE usuari_id = ?`,
        [usuari_id]
      );
      const creditsActuals = userResult.length > 0 ? userResult[0].credits : 0;

      if (creditsActuals < preuServei) {
        throw new Error("No tens prou crèdits per fer la reserva.");
      }

      const result = await conn.query(
        `INSERT INTO reserves (usuari_id, servei_id, data_reserva, estat_id, created_at, updated_at)
         VALUES (?, ?, ?, ?, NOW(), NOW())`,
        [usuari_id, servei_id, data_reserva, estat_id],
      );

      // Registrar transacción de uso de créditos
      const Transaction = require("./transactionModel");
      const saldo_final = creditsActuals - preuServei;
      await Transaction.createTransaction({
        usuari_id,
        tipus: "us",
        quantitat: -preuServei,
        saldo_final,
        descripcio: `Reserva de servei ${servei_id}`,
      });

      return {
        missatge,
        reserva: {
          id: Number(result.insertId),
          usuari_id,
          servei_id,
          data_reserva,
          estat_id,
        },
      };

    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Obté totes les reserves pendents o confirmades dels serveis d’un professional
   * @param {number} professionalId - ID del professional (ofertant)
   * @returns {Promise<Array>} - Llista de reserves
   */
  async obtenirReservesPerProfessional(professionalId) {
    let conn;
    try {
      conn = await db.getConnection();
      const query = `
        SELECT r.id AS reserva_id, r.data_reserva, r.estat_id, u.id AS usuari_id,
        u.nom AS usuari_nom, u.mail AS usuari_mail, s.id AS servei_id, s.nom AS servei_nom
        FROM reserves r INNER JOIN serveis s ON r.servei_id = s.id
        INNER JOIN usuaris u ON r.usuari_id = u.id
        WHERE s.ofertant_id = ?
        AND r.estat_id IN (1, 2)
        ORDER BY r.data_reserva DESC`;

      const result = await conn.query(query, [professionalId]);
      return result;

    } catch (error) {
      console.error("Error obtenint reserves:", error);
      throw error;

    } finally {
      if (conn) conn.release();
    }
  },

  /**
   * Obté totes les reserves pendents, confirmades o cancelades que ha realitzat un usuari
   * @param {number} usuariId - ID del usuari
   * @returns {Promise<Array>} - Llista de reserves
   */
  async obtenirReservesPerUsuari(usuariId) {
    let conn;
    try {
      conn = await db.getConnection();
      const query = `
        SELECT r.id AS reserva_id, r.data_reserva, r.estat_id,
        s.id AS servei_id, s.nom AS servei_nom,
        u.id AS usuari_id, u.nom AS usuari_nom, u.mail AS usuari_mail
        FROM reserves r
        INNER JOIN serveis s ON r.servei_id = s.id
        INNER JOIN usuaris u ON r.usuari_id = u.id
        WHERE r.usuari_id = ? AND r.estat_id != 3
        ORDER BY r.data_reserva DESC`;

      const result = await conn.query(query, [usuariId]);
      return result;

    } catch (error) {
      console.error("Error obtenint reserves:", error);
      throw error;

    } finally {
      if (conn) conn.release();
    }
  }
};

module.exports = reservaModel;

