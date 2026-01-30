const Transaction = require("../models/transactionModel");
const User = require("../models/userModel");

/**
 * Procesa un pago ficticio y añade créditos al usuario
 * @param {Request} req - Objeto de solicitud de Express
 * @param {Response} res - Objeto de respuesta de Express
 */
async function processPayment(req, res) {
  try {
    // 1) Obtener userId del middleware de autenticación
    const userId = req.user.userId;

    // 2) Obtener datos del pago
    const { quantitat, preu, metode_pagament, descripcio } = req.body;

    // Validar que la cantidad es positiva
    if (quantitat <= 0) {
      return res.status(400).json({
        error: "La quantitat ha de ser un número positiu.",
      });
    }

    // 3) Simular procesamiento (añadir delay para simular integración con gateway de pago)
    const processingTime = Math.floor(Math.random() * 1500) + 500; // Entre 500ms y 2000ms
    await new Promise((resolve) => setTimeout(resolve, processingTime));

    // 4) Simular éxito/fracaso del pago (95% probabilidad de éxito)
    const isSuccessful = Math.random() <= 0.95;

    if (!isSuccessful) {
      return res.status(400).json({
        success: false,
        error:
          "El pagament ha estat rebutjat. Si us plau, torneu-ho a provar o utilitzeu un altre mètode de pagament.",
      });
    }

    // 5) Obtener el saldo actual del usuario
    const saldoActual = await User.getCurrentCredits(userId);

    // 6) Calcular el nuevo saldo
    const saldoFinal = saldoActual + quantitat;

    // 7) Crear transacción en la base de datos
    const transaccion = await Transaction.createTransaction({
      usuari_id: userId,
      tipus: "compra",
      quantitat: quantitat,
      saldo_final: saldoFinal,
      preu: preu, // Usar el precio recibido en el body
      descripcio: `Compra de ${quantitat} crèdits amb ${metode_pagament || "targeta"}`,
    });

    // 8) Enviar respuesta exitosa
    res.status(200).json({
      success: true,
      message: "Pagament processat correctament",
      transaccio: {
        id: transaccion.id,
        quantitat,
        saldo_final: saldoFinal,
        descripcio: transaccion.descripcio,
        data: new Date(),
      },
    });
  } catch (error) {
    res.status(500).json({
      success: false,
      error: "Error en processar el pagament. Torneu-ho a provar més tard.",
    });
  }
}

module.exports = { processPayment };
