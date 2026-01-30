
## 📌 Crear una reserva (MVC)

Permet als usuaris reservar una plaça en un servei concret.

### 📎 Endpoint

```
POST /api/reserves
```

### 🔐 Autenticació

No requerida per ara (en futur es pot requerir token)

### 📥 Cos de la petició (JSON)

```json
{
  "usuari_id": 3,
  "servei_id": 5,
  "data_reserva": "2025-04-20T10:00:00"
}
```

### 📤 Resposta esperada

```json
{
  "missatge": "Reserva creada correctament",
  "reserva": {
    "id": 42,
    "usuari_id": 3,
    "servei_id": 5,
    "data_reserva": "2025-04-20T10:00:00",
    "estat_id": 1
  }
}
```

### ⚠️ Errors possibles

- `400`: Falten dades obligatòries
- `404`: Servei no trobat
- `500`: Error intern del servidor
- Personalitzat: No hi ha places disponibles

---

### 📂 Arxius relacionats

- `controllers/reservaController.js`
- `models/reservaModel.js`
- `routes/reservaRoutes.js`
- `app.js` (registre de rutes)
