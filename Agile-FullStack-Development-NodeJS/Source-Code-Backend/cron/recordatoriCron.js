// cron/recordatoriCron.js
const cron = require("node-cron");
const { executarRecordatoris } = require("../services/recordatoriService");

// ⏰ Executar cada dia a les 9:00 AM
cron.schedule("0 9 * * *", async () => {
  console.log("⏱️ Execució de recordatoris programada...");
  await executarRecordatoris();
});
