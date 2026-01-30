// app.js
const express = require("express");
const cors = require("cors");
require("dotenv").config();
require("./cron/recordatoriCron");

const app = express();

app.use(cors());
app.use(express.json());

const authRoutes = require("./routes/authRoutes");
app.use("/api/auth", authRoutes);

const logOutRoutes = require("./routes/logOutRoutes");
app.use("/api/usuari", logOutRoutes);

const esborraRoutes = require("./routes/esborraRoutes");
app.use("/api/usuari", esborraRoutes);

const userRoutes = require("./routes/userRoutes");
app.use("/api", userRoutes);

const subscripcionsRoutes = require("./routes/subscriptionsRoutes");
app.use("/api/subscripcions", subscripcionsRoutes);

const serveisRoutes = require("./routes/serveisRoutes");
app.use("/api/serveis", serveisRoutes);

const reservaRoutes = require("./routes/reservaRoutes");
app.use("/api/reserves", reservaRoutes);

const cancelReservaRoutes = require("./routes/cancelReservaRoutes");
app.use("/api/reserves", cancelReservaRoutes);

const serveisFiltratsRoutes = require("./routes/serveisFiltratsRoutes");
app.use("/api/serveis", serveisFiltratsRoutes);

const disponibilitatRoutes = require("./routes/disponibilitatRoutes");
app.use("/api/disponibilitat", disponibilitatRoutes);

const valoracionsRoutes = require("./routes/valoracionsRoutes");
app.use("/api/valoracions", valoracionsRoutes);

const challengeEnrollmentRoutes = require("./routes/challengeEnrollmentRoutes");
app.use("/api/inscripcions", challengeEnrollmentRoutes);

const challengeRoutes = require("./routes/challengeRoutes");
app.use("/api/reptes", challengeRoutes);

const rankingRoutes = require("./routes/rankingRoutes");
app.use("/api/ranking", rankingRoutes);

const productesRoutes = require('./routes/productesRoutes');
app.use('/api/productes', productesRoutes);

const pool = require("./models/db");

//Testear la conexión a la base de datos
async function testConnection() {
  try {
    console.log("Attempting to get a connection from the pool...");
    const conn = await pool.getConnection();
    console.log("Connection acquired successfully.");
    conn.release();
    console.log("Connection released.");
  } catch (error) {
    console.error("Error during connection test:", error.message);
  }
}

testConnection();

module.exports = app;
