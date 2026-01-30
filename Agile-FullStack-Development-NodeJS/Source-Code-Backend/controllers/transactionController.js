const Transaction = require("../models/transactionModel");

// Crear una nova transacció
async function createTransaction(req, res) {
  try {
    // Extrae los datos del body (ajusta según tus necesidades)
    const usuari_id = req.user.userId;
    const { tipus, quantitat, pla_subscripcio_id, preu, descripcio } = req.body;

    // Obtén el saldo anterior del usuario y calcula el saldo final
    const saldoAnterior = await Transaction.getLatestBalance(usuari_id);
    const saldo_final = saldoAnterior + quantitat;

    // Crea la transacción en la base de datos
    const newTransaction = await Transaction.createTransaction({
      usuari_id,
      tipus,
      quantitat,
      saldo_final,
      pla_subscripcio_id,
      preu,
      descripcio,
    });

    // Devuelve la respuesta
    res.status(201).json({
      message: "Transacció creada correctament",
      transaccio: newTransaction,
    });
  } catch (error) {
    console.error("Error creant la transacció:", error.message);
    res.status(500).json({
      message: "Error creant la transacció",
      error: error.message,
    });
  }
}

// Obtener todas las transacciones de un usuario
async function getTransactionsByUser(req, res) {
  try {
    // Supongamos que el ID del usuario viene como parámetro en la ruta
    // Ejemplo de ruta: GET /transactions/:usuari_id
    const usuari_id = req.user.userId;

    // Llama al modelo para obtener las transacciones
    const transactions = await Transaction.getTransactionsByUser(usuari_id);

    // Devuelve la respuesta
    res.status(200).json({
      message: "Transaccions obtingudes correctament",
      transactions,
    });
  } catch (error) {
    console.error("Error obtenint transaccions per usuari:", error.message);
    res.status(500).json({
      message: "Error obtenint transaccions per usuari",
      error: error.message,
    });
  }
}

module.exports = {
  createTransaction,
  getTransactionsByUser,
};
