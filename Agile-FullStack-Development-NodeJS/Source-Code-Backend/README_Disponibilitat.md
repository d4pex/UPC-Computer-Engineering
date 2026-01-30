
# 📅 HU #38 - Disponibilitat dels serveis

Aquesta funcionalitat permet consultar la disponibilitat horària de **tots els serveis** en una data concreta, tenint en compte la capacitat i les reserves confirmades.

---

## ✅ Endpoint

```
GET /api/disponibilitat?data=YYYY-MM-DD
```

---

## 📥 Paràmetres

| Paràmetre | Tipus   | Requerit | Exemple         | Descripció                                |
|-----------|---------|----------|------------------|--------------------------------------------|
| data      | string  | ✅        | `2025-04-25`     | Data per la qual es vol consultar disponibilitat |

---

## 🔄 Resposta

```json
[
  { "servei_id": 6, "nom": "Spinning", "hora": "08:00", "estat": "Disponible" },
  { "servei_id": 6, "nom": "Spinning", "hora": "09:00", "estat": "Ple" },
  { "servei_id": 7, "nom": "Ioga", "hora": "10:00", "estat": "Disponible" }
]
```

- L'estat pot ser:
  - `Disponible` → Encara hi ha places lliures
  - `Ple` → Capacitat completa a aquella hora

---

## 🧪 Prova amb Postman

### 🔹 Consulta general
```
GET http://localhost:3000/api/disponibilitat?data=2025-04-25
```

- Si no passes `data` o el format és incorrecte, retorna error 400.

---

## 🧠 Lògica interna

1. Només es tenen en compte serveis amb `estat_id = 1` (actius)
2. Es generen franges horàries de `08:00` a `20:00`
3. Per cada servei i hora:
   - Es compten les reserves confirmades (`estat_id = 2`)
   - Es compara amb la `capacitat` del servei
4. Es marca la franja com `Disponible` o `Ple`

---

## 📁 Fitxers implicats

| Fitxer                             | Funció                                     |
|------------------------------------|--------------------------------------------|
| `models/disponibilitatModel.js`    | Consulta serveis, franges i reserves       |
| `controllers/disponibilitatController.js` | Gestiona request i resposta         |
| `routes/disponibilitatRoutes.js`   | Defineix la ruta GET amb paràmetre `data`  |

---
