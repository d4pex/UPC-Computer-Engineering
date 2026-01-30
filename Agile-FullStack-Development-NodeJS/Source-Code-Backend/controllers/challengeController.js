const challengeModel = require("../models/challengeModel");
const userModel = require("../models/userModel");
const Transaction = require("../models/transactionModel");

const userModelMain = require("../models/userModel");

const challengeController = {
  // POST /inscripcions
  async enroll(req, res) {
    console.log("contenido de req: ", req.params);
    const usuari_id = req.user.userId;
    const { repte_id } = req.body;

    try {
      const alreadyEnrolled = await challengeModel.isEnrolled(
        usuari_id,
        repte_id,
      );
      if (alreadyEnrolled) {
        return res
          .status(409)
          .json({ missatge: "L'usuari ja està inscrit en aquest repte." });
      }

      // Obtener el precio del reto
      const preu = await challengeModel.getChallengePriceById(repte_id);

      if (preu > 0) {
        // Es de pago, comprobar créditos
        const credits = await userModel.getCurrentCredits(usuari_id);
        if (credits < preu) {
          return res.status(402).json({
            missatge: "No tens prou crèdits per inscriure't en aquest repte.",
          });
        }
        // Descontar créditos
        const quantitat = -preu; // Negativo si es gasto, 0 si es gratuito
        const saldoAnterior = await Transaction.getLatestBalance(usuari_id);
        const saldo_final = saldoAnterior + quantitat;

        await Transaction.createTransaction({
          usuari_id,
          tipus: "us",
          quantitat,
          saldo_final,
          preu,
          descripcio: `Inscripció al repte ${repte_id}`,
        });
      }

      await challengeModel.enroll(usuari_id, repte_id);
      res
        .status(201)
        .json({ missatge: "Usuari inscrit correctament al repte." });
    } catch (error) {
      console.error("Error al inscriure:", error);
      res.status(500).json({ error: "Error intern del servidor" });
    }
  },

  // GET /inscripcions/:usuari_id
  async getUserChallenges(req, res) {
    const usuari_id = req.user.userId;

    try {
      const challenges = await challengeModel.getUserChallenges(usuari_id);
      res.json(challenges);
    } catch (error) {
      console.error("Error obtenint reptes:", error);
      res.status(500).json({ error: "Error intern del servidor" });
    }
  },

  // PUT /inscripcions/completar
  async markAsCompleted(req, res) {
    const usuari_id = req.user.userId;
    const repte_id = req.params.repte_id;

    try {
      const updated = await challengeModel.markAsCompleted(usuari_id, repte_id);
      if (updated) {
        res.json({ missatge: "Repte marcat com completat." });
      } else {
        res.status(404).json({ missatge: "Inscripció no trobada." });
      }
    } catch (error) {
      console.error("Error marcant com completat:", error);
      res.status(500).json({ error: "Error intern del servidor" });
    }
  },

  // DELETE /inscripcions
  async removeEnrollment(req, res) {
    const usuari_id = req.user.userId;
    const repte_id = req.params.repte_id;

    try {
      const removed = await challengeModel.removeEnrollment(
        usuari_id,
        repte_id,
      );
      if (removed) {
        res.json({ missatge: "Inscripció eliminada correctament." });
      } else {
        res.status(404).json({ missatge: "Inscripció no trobada." });
      }
    } catch (error) {
      console.error("Error eliminant inscripció:", error);
      res.status(500).json({ error: "Error intern del servidor" });
    }
  },

  // POST /reptes
  async createChallenge(req, res) {
    const { nom, descripcio, preu, data_fi, credits_recompensa, objectiu } =
      req.body;
    try {
      const id = await challengeModel.createChallenge({
        nom,
        descripcio,
        preu,
        data_fi,
        credits_recompensa,
        objectiu,
      });
      res
        .status(201)
        .json({ id: id.toString(), missatge: "Repte creat correctament." });
    } catch (error) {
      console.error("Error creant repte:", error);
      res.status(500).json({ error: "Error intern del servidor" });
    }
  },

  async getChallenges(req, res) {
    try {
      const filters = req.query;
      const challenges = await challengeModel.getChallengesWithFilters(filters);
      res.json(challenges);
    } catch (error) {
      res.status(500).json({ error: "Error obtenint reptes" });
    }
  },

  // POST /reptes/progressar
  async progressChallengeByClass(req, res) {
    const usuari_id = req.user.userId;
    const { clase_id } = req.body;

    try {
      const updated = await challengeModel.incrementarProgresPerClasse(
        usuari_id,
        clase_id,
      );
      if (!updated) {
        return res
          .status(404)
          .json({ missatge: "No hi ha cap repte actiu per aquesta classe." });
      }
      res.status(200).json({ missatge: "Progrés actualitzat correctament." });
    } catch (error) {
      console.error("Error actualitzant progrés del repte:", error);
      res.status(500).json({ error: "Error intern del servidor" });
    }
  },

  // GET /reptes/:repte_id/progres
  async getChallengeProgress(req, res) {
    const usuari_id = req.user.userId;
    const repte_id = req.params.repte_id;

    try {
      const progress = await challengeModel.getProgress(usuari_id, repte_id);
      if (progress === null) {
        return res
          .status(404)
          .json({ missatge: "No estàs inscrit en aquest repte." });
      }
      res.json({
        repte_id,
        progress: progress.progress,
        objectiu: progress.objectiu,
      });
    } catch (error) {
      console.error("Error obtenint progrés del repte:", error);
      res.status(500).json({ error: "Error intern del servidor" });
    }
  },

  // GET /ranking/credits
  async getRankingUsuariosMasCreditos(req, res) {
    try {
      const limit = req.query.limit ? parseInt(req.query.limit) : 10;
      const ranking = await userModelMain.getRankingUsuariosMasCreditos(limit);
      res.json(ranking);
    } catch (error) {
      console.error("Error obtenint ranking d'usuaris:", error);
      res.status(500).json({ error: "Error intern del servidor" });
    }
  },

  // GET /badges
  async getUserBadges(req, res) {
    const usuari_id = req.user.userId;
    try {
      const badges = await challengeModel.getUserBadges(usuari_id);
      res.json(badges);
    } catch (error) {
      console.error("Error obtenint badges:", error);
      res.status(500).json({ error: "Error intern del servidor" });
    }
  },
};

module.exports = challengeController;
