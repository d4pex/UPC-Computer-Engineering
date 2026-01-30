const userModel = require("../models/userModel");
const bcrypt = require("bcrypt");

/**
 * Obté la informació d'un usuari pel seu ID
 * @param {Request} req - Objecte de sol·licitud d'Express
 * @param {Response} res - Objecte de resposta d'Express
 */
async function getUserById(req, res) {
  try {
    const userId = req.params.id;

    // Comprovar si l'usuari que fa la petició té dret a veure aquesta informació
    // (per exemple, només el mateix usuari o un administrador)
    if (
      req.user &&
      req.user.userId !== parseInt(userId) &&
      req.user.rol !== 1
    ) {
      // Assumint que rol 1 és admin
      return res
        .status(403)
        .json({ message: "No tens permís per accedir a aquesta informació" });
    }

    const user = await userModel.findById(userId);

    if (!user) {
      return res.status(404).json({ message: "Usuari no trobat" });
    }

    // Eliminar camps sensibles abans de retornar
    delete user.contrasenya;

    res.status(200).json(user);
  } catch (error) {
    console.error("Error obtenint usuari:", error);
    res
      .status(500)
      .json({ message: "Error al obtenir l'usuari", error: error.message });
  }
}

/**
 * Actualitza el perfil d'un usuari
 * @param {Request} req - Objecte de sol·licitud d'Express
 * @param {Response} res - Objecte de resposta d'Express
 */
async function updateUserProfile(req, res) {
  const id = req.user.userId;
  const { nom, sobrenom, mail, telefon } = req.body;

  if (!id) {
    return res.status(401).json({ message: "No autoritzat" });
  }

  // Validacions
  const telefonoRegex = /^[0-9]{9,15}$/;
  if (telefon && !telefonoRegex.test(telefon)) {
    return res.status(400).json({ message: "Número de telèfon no vàlid" });
  }

  const mailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
  if (mail && !mailRegex.test(mail)) {
    return res.status(400).json({ message: "Correu electrònic no vàlid" });
  }

  try {
    // Verificar si l'usuari existeix
    const userExists = await userModel.findById(id);
    if (!userExists) {
      return res.status(404).json({ message: "Usuari no trobat" });
    }

    // Comprovar si el correu ja està en ús per un altre usuari
    if (mail) {
      const emailExists = await userModel.checkEmailExists(mail, id);
      if (emailExists) {
        return res.status(400).json({ message: "Correu electrònic ja en ús" });
      }
    }

    // Actualitzar perfil
    const updateSuccess = await userModel.updateProfile(id, {
      nom,
      sobrenom,
      mail,
      telefon,
    });

    if (!updateSuccess) {
      return res
        .status(400)
        .json({ message: "No s'han enviat dades per actualitzar" });
    }

    res.json({ message: "Perfil actualitzat correctament" });
  } catch (error) {
    console.error("Error actualitzant perfil:", error);
    res.status(500).json({ message: "Error en el servidor" });
  }
}

/**
 * Canvia la contrasenya d'un usuari
 * @param {Request} req - Objecte de sol·licitud d'Express
 * @param {Response} res - Objecte de resposta d'Express
 */
async function changeUserPassword(req, res) {
  const id = req.user.userId;
  const { oldPass, newPass, newPassConf } = req.body;

  if (!id) {
    return res.status(401).json({ message: "No autoritzat" });
  }

  // Validacions bàsiques
  if (!oldPass || !newPass || !newPassConf) {
    return res.status(400).json({ message: "Tots els camps són obligatoris" });
  }

  if (newPass !== newPassConf) {
    return res
      .status(400)
      .json({ message: "Les noves contrasenyes no coincideixen" });
  }

  if (newPass === oldPass) {
    return res
      .status(400)
      .json({ message: "La nova contrasenya i l'antiga són la mateixa" });
  }

  // Validació de la complexitat de la contrasenya
  const passwordRegex =
    /^(?=.*[A-Za-z])(?=.*\d)(?=.*[@$!%*?&#])[A-Za-z\d@$!%*?&#]{8,}$/;
  if (!passwordRegex.test(newPass)) {
    return res.status(400).json({
      message:
        "La nova contrasenya ha de tenir almenys 8 caràcters, incloure lletres, números i un caràcter especial.",
    });
  }

  try {
    // Obtenir usuari per verificar la contrasenya actual
    const user = await userModel.findById(id);
    if (!user) {
      return res.status(404).json({ message: "Usuari no trobat" });
    }

    // Verificar la contrasenya antiga
    const isMatch = await bcrypt.compare(oldPass, user.contrasenya);
    if (!isMatch) {
      return res.status(400).json({ message: "Contrasenya actual incorrecta" });
    }

    // Generar hash de la nova contrasenya
    const hashedPassword = await bcrypt.hash(newPass, 10);

    // Actualitzar la contrasenya
    const changeSuccess = await userModel.changePassword(id, hashedPassword);

    if (!changeSuccess) {
      return res
        .status(404)
        .json({ message: "No es va poder actualitzar la contrasenya" });
    }

    res.json({ message: "Contrasenya actualitzada correctament" });
  } catch (error) {
    console.error("Error canviant la contrasenya:", error);
    res.status(500).json({ message: "Error en el servidor" });
  }
}

module.exports = { updateUserProfile, getUserById, changeUserPassword };
