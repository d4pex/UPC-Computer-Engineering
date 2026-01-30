const bcrypt = require("bcrypt");
const pool = require("../models/db");

//logout User
async function logoutUser(req, res) {
  try {
    res.clearCookie("token", {
      //si es guarda el token en una cookie, s'ha de borrar quan es tanca la sessió
      httpOnly: true, //evita que la cookie sigui accesible amb JavaScript al navegador
      secure: true, //la cookie només es transmet a connexions HTTPS
      sameSite: "Strict", //ayuda a preveure atacs CSRF, retringint que s'enviin cookies a través de peticions "cross-site"
    });
    res.status(200).json({ message: "Sessió tancada correctament" });
  } catch (error) {
    console.error("Error al tancar sessió:", error.message);
    res.status(500).json({
      message: "Error en el tancament de sessió",
      error: error.message,
    });
  }
}

module.exports = { logoutUser };
