// services/recordatoriService.js
const { obtenirReservesProperes24h } = require("../models/recordatoriModel");
const { enviarRecordatori } = require("./emailService");

async function executarRecordatoris() {
  try {
    const reserves = await obtenirReservesProperes24h();

    if (reserves.length === 0) {
      console.log(
        "No hi ha reserves a menys de 24h que no tinguin recordatori.",
      );
      return;
    }

    for (const reserva of reserves) {
      console.log(
        `🔔 Enviant recordatori a usuari ${reserva.usuari_id}, reserva ${reserva.id}`,
      );
      console.log("📧 Email detectat:", reserva.email);
      await enviarRecordatori(reserva.email, reserva);
    }
  } catch (error) {
    console.error("Error executant recordatoris:", error);
  }
}

module.exports = { executarRecordatoris };
