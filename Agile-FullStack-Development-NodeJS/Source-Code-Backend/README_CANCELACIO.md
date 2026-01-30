
## ❌ Cancel·lació de reserva

Permet a un usuari cancel·lar una reserva pròpia si encara està activa.

### 📎 Endpoint

```
PUT /api/reserves/:id/cancelar
```

### 🔐 Autenticació

Per ara, només cal passar el `usuari_id` al cos. En futur es pot protegir amb token.

### 📥 Cos de la petició (JSON)

```json
{
  "usuari_id": 3
}
```

### 📤 Resposta esperada

```json
{
  "missatge": "Reserva cancel·lada correctament"
}
```

### ⚠️ Errors possibles

- `404`: Reserva no trobada
- `403`: No tens permís per cancel·lar aquesta reserva
- `400`: La reserva no es pot cancel·lar (ja cancel·lada o completada)
- `500`: Error intern del servidor

---

### 📂 Arxius relacionats

- `controllers/cancelacioController.js`
- `routes/cancelacioRoutes.js`
- `app.js` (registre de rutes)
