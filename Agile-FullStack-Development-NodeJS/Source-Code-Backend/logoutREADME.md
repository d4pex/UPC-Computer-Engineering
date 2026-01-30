# 🔓 Endpoint: Logout d'usuari (`/api/usuari/logout`)

Aquest endpoint permet a un usuari autenticat **tancar la seva sessió** de manera segura, eliminant el token (si s'utilitza cookie) i retornant una resposta clara.

---

## 📌 Ruta

```
POST /api/usuari/logout
```

---

## 🔐 Autenticació

Cal enviar un **token JWT vàlid** per poder accedir a aquest endpoint.

**Header:**

```
Authorization: Bearer <el_teu_token_jwt>
```

---

## 🔄 Funcionament

1. El middleware `authMiddleware` comprova el token JWT.
2. El controlador `logoutUser` elimina la cookie `token` (si s'està utilitzant) i retorna un missatge de confirmació.
3. El client (frontend) pot fer servir aquesta resposta per eliminar el token del localStorage o cookies.

---

## ✅ Resposta d’èxit

```json
{
  "message": "Sessió tancada correctament"
}
```

---

## ❌ Respostes d’error

| Codi | Missatge                  | Causa                                 |
|------|----------------------------|----------------------------------------|
| 403  | No autoritzat              | No s’ha enviat cap token JWT           |
| 500  | Error en el tancament...   | Error intern de servidor               |

---

## 🧪 Exemple de prova amb Postman

1. **Mètode:** `POST`
2. **URL:** `http://localhost:3000/api/usuari/logout`
3. **Header:**
   ```
   Authorization: Bearer <el_teu_token_jwt>
   ```
4. **Body:** No cal enviar body.

---

## 🧠 Notes tècniques

- El token es pot eliminar com a cookie (`res.clearCookie("token")`).
- Si s’utilitza `localStorage` al frontend, aquest ha d’esborrar el token manualment després de rebre la resposta d’èxit.
- El middleware `authMiddleware` assegura que només usuaris autenticats poden accedir.

---

## 👨‍💻 Desenvolupat a la branca `logout`

Aquest endpoint ha estat implementat per oferir una forma segura de finalitzar la sessió d’usuari dins del projecte **Mou-te+**.
