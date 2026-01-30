// models/CancelReservaModel.js
const db = require("../models/db");

async function cancelarReservaDB(reservaId, usuari_id) {
  let conn;
  try {
    conn = await db.getConnection();

    const reserves = await conn.query(`SELECT * FROM reserves WHERE id = ?`, [
      reservaId,
    ]);

    if (!reserves || reserves.length === 0) {
      const error = new Error("Reserva no trobada");
      error.status = 404;
      throw error;
    }

    const reserva = reserves[0];

    if (reserva.usuari_id !== usuari_id) {
      const error = new Error("No tens permís per cancel·lar aquesta reserva");
      error.status = 403;
      throw error;
    }

    if (reserva.estat_id === 3) {
      const error = new Error(
        "La reserva no es pot cancel·lar, ja està cancel·lada.",
      );
      error.status = 400;
      throw error;
    }

    await conn.query(
      `UPDATE reserves SET estat_id = 3, updated_at = NOW() WHERE id = ?`,
      [reservaId],
    );

    return { missatge: "Reserva cancel·lada correctament" };
  } finally {
    if (conn) conn.release(); // Alliberem connexió si s’ha obtingut
  }
}

module.exports = {
  cancelarReservaDB,
};