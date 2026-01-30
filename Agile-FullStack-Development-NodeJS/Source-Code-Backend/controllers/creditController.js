const User = require("../models/userModel");

async function getUserCredits(req, res) {
  try {
    const userId = req.user.userId; // ID del usuari autenticat des del token
    console.log("id: ", userId);
    const credits = await User.getCurrentCredits(userId);

    if (credits === undefined) {
      return res.status(404).json({
        message: "No s'han trobat crèdits per a aquest usuari.",
      });
    }

    res.status(200).json({ credits });
  } catch (error) {
    console.error("Error al obtenir els crèdits de l'usuari:", error.message);
    res.status(500).json({
      message: "Error intern del servidor",
      error: error.message,
    });
  }
}

module.exports = { getUserCredits };
