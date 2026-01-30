
# 🔎 HU #37 - Filtrar serveis per categoria

Aquesta funcionalitat permet obtenir serveis filtrats pel seu tipus/categoria mitjançant un paràmetre de consulta.

---

## ✅ Endpoint

```
GET /api/serveis/filtrats?categoria=Salut
```

---

## 🧾 Paràmetres suportats

| Paràmetre   | Tipus   | Requerit | Exemple       | Descripció                                  |
|-------------|---------|----------|----------------|----------------------------------------------|
| categoria   | string  | Opcional | `categoria=Esportiu` | Nom de la categoria a filtrar (`tipus_servei.nombre`) |

---

## 🔄 Resposta esperada

```json
[
  {
    "id": 3,
    "nom": "Ioga Matinal",
    "descripcio": "Classe suau de ioga per començar el dia",
    "tipus_servei_id": 2,
    ...
  },
  ...
]
```

---

## 🧪 Prova amb Postman

### 🔹 Sense filtre
```
GET http://localhost:3000/api/serveis/filtrats
```

### 🔹 Amb filtre per categoria
```
GET http://localhost:3000/api/serveis/filtrats?categoria=Salut
```

---

## 💡 Notes

- Si no s’especifica `categoria`, retorna tots els serveis
- Si s’especifica una `categoria` inexistent, retorna array buit `[]`

---

## 🛠️ Fitxers implicats

| Fitxer                                | Funció                                 |
|---------------------------------------|----------------------------------------|
| `models/serveisFiltratsModel.js`      | Consulta a BBDD amb filtre             |
| `controllers/serveisFiltratsController.js` | Rep `req.query.categoria` i respon   |
| `routes/serveisFiltratsRoutes.js`     | Defineix ruta GET amb paràmetre        |

---
