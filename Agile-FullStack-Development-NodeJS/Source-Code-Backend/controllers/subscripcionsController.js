const subscriptionModel = require("../models/subscriptionModel");
const userModel = require("../models/userModel");

/**
 * Selecciona o actualitza la subscripció d'un usuari
 * @param {Request} req - Objecte de sol·licitud d'Express
 * @param {Response} res - Objecte de resposta d'Express
 */
async function seleccionarSubscripcio(req, res) {
  const { usuari_id, subscripcio_id } = req.body;

  // Validar els IDs
  if (!Number.isInteger(usuari_id) || !Number.isInteger(subscripcio_id)) {
    return res
      .status(400)
      .json({ error: "usuari_id i subscripcio_id han de ser números enters" });
  }

  try {
    // 1. Verificar existència d'usuari utilitzant userModel
    const userExists = await userModel.findById(usuari_id);
    if (!userExists) {
      return res.status(404).json({ error: "Usuari no trobat" });
    }

    // 2. Verificar existència de subscripció
    const subscription =
      await subscriptionModel.getSubscriptionById(subscripcio_id);
    if (!subscription) {
      return res.status(404).json({ error: "Subscripció no trobada" });
    }

    // 3. Comprovar si ja té la subscripció actual
    const currentSubscription =
      await subscriptionModel.getUserSubscription(usuari_id);
    if (
      currentSubscription &&
      currentSubscription.subscripcio_id === subscripcio_id
    ) {
      return res
        .status(400)
        .json({ error: "L'usuari ja té aquesta subscripció" });
    }

    // 4. Actualitzar subscripció i crèdits
    const credits = subscription.nbCredits || subscription.credits || 0;
    await subscriptionModel.setUserSubscription(
      usuari_id,
      subscripcio_id,
      credits,
    );

    res.status(200).json({ message: "Subscripció actualitzada correctament" });
  } catch (error) {
    console.error("Error durant la selecció de subscripció:", error.message);
    res.status(500).json({
      message: "Error seleccionant la subscripció",
      error: error.message,
    });
  }
}

module.exports = { seleccionarSubscripcio };
