
# 🕑 Recordatoris automàtics de reserves - Mou-te+

## 🔖 HU #28 - Recordatoris
Funcionalitat per detectar reserves confirmades a menys de 24 hores i enviar correus de recordatori automàticament.

---

## 🗂️ Estructura del sistema

```
moute-backend/
├── models/
│   └── recordatoriModel.js
├── services/
│   ├── recordatoriService.js
│   ├── emailService.js
│   └── testRecordatoris.js
├── cron/
│   └── recordatoriCron.js
```

---

## 📦 Dependències necessàries

```bash
npm install nodemailer node-cron
```

---

## 🔐 Variables d'entorn (.env)

```env
EMAIL_USER=el_teu_correu@gmail.com
EMAIL_PASS=contrasenya_app_gmail

DB_HOST=localhost
DB_USER=root
DB_PASSWORD=la_teva_contrasenya
DB_NAME=amep09
```

---

## 🔧 Descripció dels components

### ✅ `recordatoriModel.js`
Consulta les reserves confirmades (`estat_id = 2`) dins de les pròximes 24h i obté el `mail` del usuari com `email`.

### ✅ `recordatoriService.js`
Executa la lògica:
- Crida el model
- Envia el correu amb `nodemailer`
- Mostra per consola el procés

### ✅ `emailService.js`
Configura i envia el correu electrònic.

### ✅ `recordatoriCron.js`
Cron job que executa `executarRecordatoris()` automàticament cada dia a les **09:00 h**.

### ✅ `testRecordatoris.js`
Fitxer de test manual per executar el servei sense cron.

---

## ⏰ Configuració del cron job

A `recordatoriCron.js`:

```js
cron.schedule('0 9 * * *', async () => {
  await executarRecordatoris();
});
```

Assegura't d'incloure’l a `app.js`:

```js
require('./cron/recordatoriCron');
```

---

## 🧪 Proves manuals amb Postman

No cal Postman per aquesta funcionalitat. Es prova amb:

```bash
node services/testRecordatoris.js
```

### ✅ Resultats esperats

- Si hi ha reserva confirmada < 24h → s’envia correu
- Si no → missatge: `No hi ha reserves a menys de 24h...`

---

## 🧪 Crear reserva de prova (SQL)

```sql
INSERT INTO reserves (usuari_id, servei_id, data_reserva, estat_id, created_at, updated_at)
VALUES (11, 6, DATE_ADD(NOW(), INTERVAL 1 HOUR), 2, NOW(), NOW());

UPDATE usuaris SET mail = 'destinatari@exemple.com' WHERE id = 11;
```

---

## ✅ Exemple de sortida per consola

```
🔔 Enviant recordatori a usuari 11, reserva 6
✉️ Correu enviat a destinatari@exemple.com
```

---

## ✉️ Format del correu

Assumpte:
```
📅 Recordatori de la teva reserva
```

Cos:
```
Hola! Et recordem que tens una reserva confirmada el dia 2025-04-22T11:00:00.

Gràcies per utilitzar Mou-te+.
```

---
