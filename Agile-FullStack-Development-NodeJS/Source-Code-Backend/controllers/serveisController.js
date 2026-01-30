// controllers/serveisController.js
const { getAllServeis } = require("../models/serveisModel");

async function getServeis(req, res) {
  try {
    const serveis = await getAllServeis(req.query);
    res.status(200).json({ serveis });
  } catch (error) {
    console.error("Error obtenint serveis:", error);
    res.status(500).json({ error: "Error intern del servidor" });
  }
}

const { crearServei } = require('../models/serveisModel');

const crearServeiController = async (req, res) => {
  const { userId: ofertant_id } = req.user || {};
  const { nom, descripcio, data, ubicacio, preu, duracio_mins, capacitat, estat_id, tipus_servei_id } = req.body;

  if (!ofertant_id) {
    return res.status(401).json({ message: 'Usuari no autenticat correctament' });
  } else if (!nom || !data || !ubicacio || !preu || !duracio_mins || !capacitat || !estat_id || !tipus_servei_id) {
    return res.status(400).json({ message: 'Falten camps obligatoris' });
  } else if (preu < 0) {
    return res.status(400).json({ message: 'Valor del preu incorrecte' });
  } else if (capacitat < 0) {
    return res.status(400).json({ message: 'Valor de la capacitat incorrecte' });
  } else if (duracio_mins < 1) {
    return res.status(400).json({ message: 'Valor de la duració incorrecte' });
  } else if (estat_id < 1 || estat_id > 2) {
    return res.status(400).json({ message: 'Estat_id fora de rang' });
  } else if (tipus_servei_id < 1 || tipus_servei_id > 4) {
    return res.status(400).json({ message: 'Tipus_servei_id fora de rang' });
  }

  try {
    const serveiResult = await crearServei({
      ofertant_id,
      nom,
      descripcio,
      data,
      ubicacio,
      preu,
      duracio_mins,
      capacitat,
      estat_id,
      tipus_servei_id
    });

    res.status(201).json({ message: 'Servei creat correctament', serveiId: Number(serveiResult.insertId) });
  } catch (error) {
    console.error('Error al crear servei:', error);
    res.status(500).json({ message: 'Error del servidor' });
  }
};

const { actualitzarServei } = require('../models/serveisModel');

const modificarServei = async (req, res) => {
    const { userId: ofertant_id } = req.user || {};
    const serveiId = req.params.id;
    if(!ofertant_id || !serveiId) {
        return res.status(400).json({ message: 'Usuari o servei no identificat correctament' });
    }
    const dadesModificacio = {
    } = req.body;
  
    if (dadesModificacio.preu && dadesModificacio.preu < 0) {
        return res.status(400).json({ message: 'Valor del preu incorrecte' });
    } else if (dadesModificacio.capacitat && dadesModificacio.capacitat < 0) {
        return res.status(400).json({ message: 'Valor de la capacitat incorrecte' });
    } else if (dadesModificacio.duracio_mins && dadesModificacio.duracio_mins < 1) {
        return res.status(400).json({ message: 'Valor de la duració incorrecte' });
    } else if (dadesModificacio.estat_id && (dadesModificacio.estat_id < 1 || dadesModificacio.estat_id > 2)) {
        return res.status(400).json({ message: 'Estat_id fora de rang' });
    } else if (dadesModificacio.tipus_servei_id && (dadesModificacio.tipus_servei_id < 1 || dadesModificacio.tipus_servei_id > 4)) {
        return res.status(400).json({ message: 'Tipus_servei_id fora de rang' });
    } else if (Object.keys(dadesModificacio).length === 0) {
      return res.status(400).json({ message: 'No ha sigut proporcionat cap camp per modificar' });
    }
  
    try {
      const filesModificades = await actualitzarServei(serveiId, ofertant_id, dadesModificacio);
  
      if (filesModificades === 0) {
        return res.status(403).json({ message: 'No tens permisos per modificar aquest servei o no existeix' });
      }
  
      res.status(200).json({ message: 'Servei modificat correctament' });
    } catch (error) {
      console.error('Error al modificar servei:', error);
      res.status(500).json({ message: 'Error del servidor' });
    }
  };

  module.exports = { getServeis, modificarServei, crearServei: crearServeiController };
