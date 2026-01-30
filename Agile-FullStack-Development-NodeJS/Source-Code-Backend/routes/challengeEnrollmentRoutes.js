const express = require("express");
const router = express.Router();
const challengeEnrollmentController = require("../controllers/challengeController");

const authMiddleware = require("../middleware/authMiddleware");

// Inscribir usuario a un reto
router.post("/", authMiddleware, challengeEnrollmentController.enroll);

// Obtener todos los retos inscritos de un usuario
router.get(
  "/",
  authMiddleware,
  challengeEnrollmentController.getUserChallenges,
);

// Marcar reto como completado
router.put(
  "/:repte_id/completar",
  authMiddleware,
  challengeEnrollmentController.markAsCompleted,
);

// Eliminar inscripción de un usuario a un reto
router.delete(
  "/:repte_id",
  authMiddleware,
  challengeEnrollmentController.removeEnrollment,
);

router.get(
  "/:repte_id/progres",
  authMiddleware,
  challengeEnrollmentController.getChallengeProgress,
);

router.get(
  "/badges",
  authMiddleware,
  challengeEnrollmentController.getUserBadges,
);

module.exports = router;
