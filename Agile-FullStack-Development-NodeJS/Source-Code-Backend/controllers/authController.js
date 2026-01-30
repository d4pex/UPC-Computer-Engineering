const bcrypt = require("bcrypt");
const jwt = require("jsonwebtoken");
const userModel = require("../models/userModel");
const roleModel = require("../models/rolModel");

// Registro (revisar y completar)
async function registerUser(req, res) {
  const { dni, sobrenom, contrasenya, nom, mail, telefon, rol } = req.body;

  try {
    // 1. Comprobar si el usuario ya está registrado
    const existingUser = await userModel.exists(dni, mail);
    if (existingUser) {
      return res.status(400).json({ message: "El usuario ya está registrado" });
    }

    // 2. Obtener el rol_id correspondiente al rol
    console.log("Fetching role ID for role:", rol);
    const rolId = await roleModel.getIdByName(rol);
    if (!rolId) {
      return res.status(400).json({ message: "Rol no trobat" });
    }

    // 3. Registrar el usuario
    await userModel.register({
      dni,
      sobrenom,
      contrasenya,
      nom,
      mail,
      telefon,
      rol,
      rolId,
    });

    res.status(201).json({ message: "Usuario registrado correctamente" });
  } catch (error) {
    console.error("Error during user registration:", error.message);
    res.status(500).json({
      message: "Error registrando el usuario",
      error: error.message,
    });
  }
}

// Login (revisar y implementar posibles mejoras en seguridad)
async function loginUser(req, res) {
  const { dni, contrasenya } = req.body;

  try {
    // 1. Buscar el usuario en la tabla usuaris
    const user = await userModel.findByDni(dni);
    if (!user) {
      return res.status(404).json({ message: "Usuari no trobat" });
    }

    // 2. Comparar la contraseña hasheada
    const isPasswordValid = await bcrypt.compare(contrasenya, user.contrasenya);

    if (!isPasswordValid) {
      return res.status(401).json({ message: "Dades incorrectes" });
    }

    // 3. Generar el token JWT
    const token = jwt.sign(
      { userId: user.id, rol: user.rol_id },
      process.env.JWT_SECRET,
      {
        expiresIn: "1h",
      },
    );

    // 4. Enviar respuesta con el token
    res.status(200).json({ message: "Succesful login", token });
  } catch (error) {
    console.error("Error during login:", error.message);
    res.status(500).json({
      message: "Error al iniciar sessió",
      error: error.message,
    });
  }
}

module.exports = { registerUser, loginUser };
