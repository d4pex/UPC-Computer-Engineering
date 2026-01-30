const db = require('./db');

/**
 * Retorna la disponibilitat de tots els serveis actius per a una data concreta.
 * Només es mostren serveis programats per a aquell dia.
 * Comprova si el nombre de reserves fetes coincideix o supera la capacitat.
 *
 * @param {string} data Data en format YYYY-MM-DD
 * @returns {Array} Serveis amb el seu estat ("Disponible" o "Ple")
 */
async function obtenirDisponibilitatPerData(data) {
  const conn = await db.getConnection();
  try {
    // Obté serveis actius per la data concreta
    const serveis = await conn.query(`
      SELECT s.*, t.nombre AS tipus_servei
      FROM serveis s
      JOIN tipus_servei t ON s.tipus_servei_id = t.id
      WHERE s.estat_id = 1 AND DATE(s.data) = ?
    `, [data]);

    // Obté el nombre de reserves actives per cada servei en aquella data
    const reserves = await conn.query(`
      SELECT servei_id, COUNT(*) AS total
      FROM reserves
      WHERE DATE(data_reserva) = ?
        AND estat_id = 2
      GROUP BY servei_id
    `, [data]);

    // Mapa ràpid de reserves per servei
    const reservesMap = {};
    for (const r of reserves) {
      reservesMap[r.servei_id] = Number(r.total);
    }

    // Afegeix l'estat de disponibilitat
    const resultat = serveis.map(servei => {
      const totalReserves = reservesMap[servei.id] || 0;
      const estat = totalReserves < servei.capacitat ? 'Disponible' : 'Ple';

      return {
        ...servei,
        estat
      };
    });

    return resultat;
  } finally {
    if (conn) conn.release();
  }
}

module.exports = {
  obtenirDisponibilitatPerData
};