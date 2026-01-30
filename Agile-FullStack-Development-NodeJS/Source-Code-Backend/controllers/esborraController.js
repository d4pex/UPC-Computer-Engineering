const bcrypt = require("bcrypt");
const userModel = require("../models/userModel");

/**
 * Elimina el compte d'un usuari després de verificar la seva contrasenya
 * @param {Request} req - Objecte de sol·licitud d'Express
 * @param {Response} res - Objecte de resposta d'Express
 */
async function deleteAccount(req, res) {
  try {
    // 1) Obtenim userId del middleware d'autenticació
    const userId = req.user.userId;

    // 2) Confirmar l'acció rebent la contrasenya al body
    const { contrasenya } = req.body;

    if (!contrasenya) {
      return res.status(400).json({
        error: "La contrasenya és requerida per confirmar l'eliminació.",
      });
    }

    // 3) Obtenir el hash de la contrasenya de l'usuari
    const storedHash = await userModel.getPasswordHash(userId);

    if (!storedHash) {
      return res.status(404).json({ error: "Usuari no trobat." });
    }

    // 4) Verificar la contrasenya amb bcrypt
    const match = await bcrypt.compare(contrasenya, storedHash);
    if (!match) {
      return res.status(401).json({ error: "Contrasenya incorrecta." });
    }

    // 5) Eliminar l'usuari
    await userModel.deleteUser(userId);

    // 6) Netejar el token (opcional, si el token es guarda en cookie)
    res.clearCookie("token");

    return res.status(200).json({ message: "Compte eliminat correctament." });
  } catch (error) {
    console.error("Error eliminant el compte:", error);
    return res.status(500).json({
      error: "Error al eliminar el compte. Torna-ho a provar més tard.",
    });
  }
}

module.exports = { deleteAccount };
