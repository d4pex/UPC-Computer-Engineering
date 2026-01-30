// services/emailService.js
const nodemailer = require("nodemailer");

const transporter = nodemailer.createTransport({
  service: "gmail",
  auth: {
    user: process.env.EMAIL_USER, // definir al .env
    pass: process.env.EMAIL_PASS, // definir al .env
  },
});

async function enviarRecordatori(destinatariEmail, reserva) {
  const mailOptions = {
    from: "Mou-te+ <no-reply@moute.cat>",
    to: destinatariEmail,
    subject: "📅 Recordatori de la teva reserva",
    text: `Hola! Et recordem que tens una reserva confirmada el dia ${reserva.data_reserva}.\n\nGràcies per utilitzar Mou-te+.`,
  };

  await transporter.sendMail(mailOptions);
  console.log(`✉️ Correu enviat a ${destinatariEmail}`);
}

module.exports = { enviarRecordatori };
