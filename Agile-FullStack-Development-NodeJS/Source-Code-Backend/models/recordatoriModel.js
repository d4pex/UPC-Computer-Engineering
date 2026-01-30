// models/recordatoriModel.js
const db = require("./db");

async function obtenirReservesProperes24h() {
  let conn;
  try {
    conn = await db.getConnection();

    const reserves = await conn.query(
      `SELECT r.*, u.mail AS email FROM reserves r
       JOIN usuaris u ON r.usuari_id = u.id
       WHERE r.estat_id = 2
       AND r.data_reserva BETWEEN NOW() AND DATE_ADD(NOW(), INTERVAL 24 HOUR)`
      /*
       AND NOT EXISTS (
         SELECT 1 FROM recordatoris_enviats re
         WHERE re.reserva_id = r.id
       ) Per registrar recordatoris i evitar duplicats
       */
    );

    return reserves;
  } finally {
    if (conn) conn.release();
  }
}

module.exports = {
  obtenirReservesProperes24h,
};