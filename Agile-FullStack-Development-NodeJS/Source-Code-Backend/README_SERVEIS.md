
## 🔍 Visualització de Serveis

Permet consultar serveis disponibles amb filtres, paginació i ordenació.

### 📎 Endpoint

```
GET /api/serveis
```

### 🔐 Autenticació

No requerida.

### 📥 Paràmetres opcionals (query)

| Paràmetre          | Tipus    | Descripció                                                                 |
|--------------------|----------|-----------------------------------------------------------------------------|
| `page`             | number   | Número de pàgina (per defecte: 1)                                           |
| `limit`            | number   | Quantitat de serveis per pàgina (per defecte: 10)                           |
| `nom`              | string   | Filtra serveis pel nom (ex: "Yoga")                                        |
| `tipus_servei_id`  | number   | Filtra per tipus de servei                                                 |
| `estat_id`         | number   | Filtra per estat (actiu, inactiu, etc.)                                    |
| `preu_min`         | number   | Preu mínim                                                                 |
| `preu_max`         | number   | Preu màxim                                                                 |
| `order_by`         | string   | Camp pel qual ordenar (ex: `preu`, `nom`, `created_at`)                    |
| `order_direction`  | string   | Direcció: `asc` o `desc` (per defecte: `desc`)                             |

---

### 🧪 Exemple de crida

```
GET /api/serveis?page=1&limit=5&tipus_servei_id=1&preu_min=5&preu_max=20&order_by=preu&order_direction=asc
```

---

### 📤 Resposta esperada

```json
{
  "serveis": [
    {
      "id": 29,
      "ofertant_id": 28,
      "nom": "Classe de tennis",
      "descripcio": "Exercicis per practicar la volea",
      "data": "2025-05-07T08:00:00.000Z",
      "ubicacio": "Gavà",
      "preu": 10,
      "duracio_mins": 120,
      "capacitat": 2,
      "estat_id": 1,
      "tipus_servei_id": 1,
      "created_at": "2025-05-05T09:49:59.000Z",
      "updated_at": "2025-05-05T09:49:59.000Z"
    }
  ]
}
```

---

### ⚠️ Notes

- Si no es passen paràmetres, es retornen els 10 primers serveis ordenats per `created_at DESC`.
- Si no hi ha resultats, es retornarà una llista buida:
  ```json
  { "serveis": [] }
  ```

---

### 📍 Desenvolupament relacionat

- Fitxer de ruta: `routes/serveisRoutes.js`
- Fitxer controlador: `controllers/serveisController.js`
