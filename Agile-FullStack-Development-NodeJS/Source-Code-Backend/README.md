# Moute Backend

Este proyecto es un servidor backend construido con Node.js y Express. Proporciona funcionalidades de autenticación y registro de usuarios, así como la conexión a una base de datos MariaDB.

## Requisitos

Antes de comenzar, asegúrate de tener instalado lo siguiente:

- [Node.js](https://nodejs.org/) (versión 14 o superior)
- [npm](https://www.npmjs.com/) (viene con Node.js)
- [MariaDB](https://mariadb.org/)

## Instalación

1. Clona el repositorio:

   ```bash
   git clone https://github.com/tu-usuario/moute-backend.git
   cd moute-backend
   ```

2. Instala las dependencias:

   ```bash
   npm install
   ```

3. Crea un archivo `.env` en la raíz del proyecto y añade las siguientes variables de entorno:

   ```env
   DB_HOST=localhost
   DB_USER=tu_usuario
   DB_PASSWORD=tu_contraseña
   DB_NAME=nombre_de_tu_base_de_datos
   ```

   Asegúrate de reemplazar `tu_usuario`, `tu_contraseña` y `nombre_de_tu_base_de_datos` con los valores correspondientes a tu configuración de MariaDB.

## Uso

1. Inicia el servidor:

   ```bash
   npm start
   ```

   El servidor debería estar corriendo en `http://localhost:3000`.

2. Puedes probar las rutas de autenticación usando herramientas como [Postman](https://www.postman.com/) o [cURL](https://curl.se/).

### Rutas disponibles

- **Registro de usuario**

  - **URL:** `/api/auth/register`
  - **Método:** `POST`
  - **Cuerpo de la solicitud:**

    ```json
    {
      "dni": "12345678A",
      "sobrenom": "usuario",
      "contrasenya": "password123",
      "nom": "Nombre",
      "mail": "usuario@example.com",
      "telefon": "123456789",
      "rol": "Usuario"
    }
    ```

- **Inicio de sesión**

  - **URL:** `/api/auth/login`
  - **Método:** `POST`
  - **Cuerpo de la solicitud:**

    ```json
    {
      "dni": "12345678A",
      "contrasenya": "password123"
    }
    ```


- **Selección de plan**

  - **URL:** `/api/subscripcions/seleccionar`
  - **Método:** `PUT`
  - **Cuerpo de la solicitud:**

    ```json
    {
      "usuari_id": 11,
      "subscripcio_id": 2
    }
    ```
- **Obtener usuario**

  - **URL:** `/api/usuari/:id`
  - **Método:** `GET`
  - **Ejemplo: quiero saber la información del usuario con id=11 `/api/usuari/11`**

- **Modificar perfil**

  - **URL:** `/api/usuari/actualitzar`
  - **Método:** `PUT`
  - **Cuerpo de la solicitud:**

    ```json
    {
      "nom": "nou nom",
      "sobrenom": "nou sobrenom",
      "mail": "noumail@mail.com", // Formato mail: x@x.x
      "telefon": "nou tlf" // Formato tlf: [0-9], 9-15 carácteres
    }
    ```

- **Modificar contraseña**

  - **URL:** `/api/usuari/canviar-contrasenya`
  - **Método:** `PUT`
  - **Cuerpo de la solicitud:**

    ```json
    {
      "oldPass": "contrasenya antiga",
      "newPass": "nova contrasenya",
      "newPassConf": "repetir nova contrasenya"
      // Formato contraseña: <=8 carácteres, letras y
      // números, <= 1 carácter especial: [@$!%*?&#]
    }
    ```
- **Logout**

  - **URL:** `/api/usuari/logout`
  - **Método:** `POST`
  - **Headers de la solicitud:**

    ```json
    {
      Authorization: Bearer <token_jwt>
    }
    ```
- **Esborrar Compte**

  - **URL:** `/api/usuari/eliminar`
  - **Método:** `POST`
  - **Cuerpo de la solicitud:**

    ```json
    {
      "contrasenya": "contrasenya_actual"
    }
    ```

## Gestión de Créditos

### Consultar Créditos

Obtiene los créditos disponibles del usuario autenticado.

- **URL:** `/api/usuari/credits`
- **Método:** `GET`
- **Autenticación:** Requerida (JWT Token)
- **Headers:**
  ```
  Authorization: Bearer <token_jwt>
  ```
- **Respuesta exitosa:**
  ```json
  {
    "credits": 150
  }
  ```

---

## Ranking de usuarios con más créditos

El frontend puede mostrar el ranking de los usuarios con más créditos de la plataforma usando el siguiente endpoint:

- **URL:** `/api/ranking`
- **Método:** `GET`
- **Query params opcional:**
  - `limit`: Número de usuarios a devolver (por defecto 10)
- **Respuesta exitosa:**
  ```json
  [
    {
      "id": 1,
      "nom": "Carlos",
      "sobrenom": "carlitos",
      "mail": "carlos@example.com",
      "credits": 150
    },
    {
      "id": 2,
      "nom": "Maria",
      "sobrenom": "mari",
      "mail": "maria@example.com",
      "credits": 120
    }
    // ...
  ]
  ```
- Los usuarios se ordenan de mayor a menor cantidad de créditos.

### Cómo usarlo desde el frontend

- Realiza una petición GET a `/api/ranking/credits` (puedes añadir `?limit=5` para limitar el número de resultados).
- Muestra la lista de usuarios en una tabla, ranking, podio, etc.
- Los campos disponibles para mostrar son: `nom`, `sobrenom`, `mail`, y `credits`.
- Este endpoint no requiere autenticación, por lo que puede usarse para mostrar rankings públicos.

---

## Gestión de Retos (Reptes)

### Listar y filtrar retos

- **URL:** `/api/reptes`
- **Método:** `GET`
- **Query params opcionales:**
  - `preu_min`, `preu_max`: Filtrar por créditos de coste
  - `recompensa_min`, `recompensa_max`: Filtrar por créditos de recompensa
  - `data_fi`: Filtrar por fecha de finalización máxima
  - `estat`: Filtrar por estado (abierto/cerrado)
- **Ejemplo:**
  `/api/reptes?preu_min=0&preu_max=10&estat=obert&data_fi=2024-12-31`

### Crear un reto

- **URL:** `/api/reptes`
- **Método:** `POST`
- **Body:**
  ```json
  {
    "nom": "Reto Gratuito",
    "descripcio": "Un reto sin coste",
    "preu": 0,
    "data_fi": "2024-12-31",
    "credits_recompensa": 10
  }
  ```

---

## Inscripción a Retos

> Todas estas rutas requieren autenticación (JWT en el header Authorization).

---

## Consulta de Badges (Medallas) del Usuario

Permite al usuario autenticado consultar sus badges (retos completados).

- **URL:** `/api/inscripcions/badges`
- **Método:** `GET`
- **Autenticación:** Requerida (JWT Token)
- **Headers:**
  ```
  Authorization: Bearer <token_jwt>
  ```
- **Respuesta exitosa:**
  ```json
  [
    {
      "id": 1,
      "nom": "Reto Gratuito",
      "descripcio": "Un reto sin coste",
      "preu": 0,
      "data_fi": "2024-12-31T00:00:00.000Z",
      "credits_recompensa": 10,
      "objectiu": 5,
      // ...otros campos del reto/badge
    },
    // ...
  ]
  ```
- Devuelve la lista de retos completados (badges) del usuario autenticado.

---

### Inscribirse a un reto

- **URL:** `/api/inscripcions`
- **Método:** `POST`
- **Body:**
  ```json
  {
    "repte_id": 1
  }
  ```
- **Respuesta:**
  - 201: Inscripción correcta
  - 402: No tienes suficientes créditos
  - 409: Ya inscrito

### Ver retos inscritos

- **URL:** `/api/inscripcions`
- **Método:** `GET`
- **Respuesta:**
  Lista de retos en los que el usuario autenticado está inscrito.

### Marcar reto como completado

- **URL:** `/api/inscripcions/:repte_id/completar`
- **Método:** `PUT`

### Desapuntarse de un reto

- **URL:** `/api/inscripcions/:repte_id`
- **Método:** `DELETE`
- **Respuesta:**
  - 200: Inscripción eliminada
  - 404: No estabas inscrito

---

## Procesamiento de Pagos

### Realizar un Pago

Procesa un pago para añadir créditos a la cuenta del usuario.

- **URL:** `/api/usuari/pagament`
- **Método:** `POST`
- **Autenticación:** Requerida (JWT Token)
- **Headers:**
  ```
  Content-Type: application/json
  Authorization: Bearer <token_jwt>
  ```
- **Cuerpo de la solicitud:**
  ```json
  {
    "quantitat": 10,
    "metode_pagament": "targeta",
    "descripcio": "Compra de 10 crèdits" // Opcional
  }
  ```
- **Parámetros:**
  - `quantitat`: (Number) Cantidad de créditos a comprar
  - `metode_pagament`: (String) Método de pago ("targeta", "paypal", etc.)
  - `descripcio`: (String, opcional) Descripción personalizada del pago

- **Respuesta exitosa (200):**
  ```json
  {
    "success": true,
    "message": "Pagament processat correctament",
    "transaccio": {
      "quantitat": 10,
      "saldo_final": 160,
      "descripcio": "Compra de 10 crèdits amb targeta",
      "data": "2023-11-08T15:45:30.123Z"
    }
  }
  ```

- **Respuesta de error (400):**
  ```json
  {
    "error": "La quantitat ha de ser un número positiu."
  }
  ```
  o
  ```json
  {
    "success": false,
    "error": "El pagament ha estat rebutjat. Si us plau, torneu-ho a provar o utilitzeu un altre mètode de pagament."
  }
  ```

---

## Funcionalidades avanzadas de retos y reservas

### Progreso automático de retos, badges y recompensas al reservar

Cuando un usuario realiza una reserva de clase/servicio, el sistema:

- Comprueba si el usuario está inscrito en algún reto asociado a la clase reservada.
- Si está inscrito, incrementa automáticamente el campo `progress` de la inscripción al reto.
- Si el progreso alcanza el objetivo (`objectiu` del reto), la inscripción se marca automáticamente como completada (`completat = 1`).
- Si el reto tiene recompensa (`credits_recompensa > 0`), el usuario recibe automáticamente los créditos de recompensa y se registra una transacción de tipo "recompensa".
- El usuario recibe también el badge (medalla) correspondiente al reto completado.

No es necesario que el usuario realice ninguna acción adicional: el progreso, la recompensa y el badge se actualizan de forma transparente al reservar.

### Consultar progreso y objetivo de un reto

- **URL:** `/api/reptes/:repte_id/progres`
- **Método:** `GET`
- **Autenticación:** Requerida (JWT Token)
- **Respuesta exitosa:**
  ```json
  {
    "repte_id": 5,
    "progress": 3,
    "objectiu": 10
  }
  ```
- Si el usuario no está inscrito en el reto, devuelve un error 404.

### Ejemplo de flujo automático

1. El usuario se inscribe a un reto (por ejemplo, "Asiste a 5 clases de yoga").
2. Cada vez que reserva una clase de yoga, el progreso del reto aumenta automáticamente.
3. Cuando el progreso alcanza el objetivo:
   - El reto se marca como completado.
   - El usuario recibe el badge correspondiente al reto.
   - Si el reto tiene recompensa, el usuario recibe los créditos y se registra una transacción de tipo "recompensa".

---

## Gestión de Transacciones

### Crear Transacción

Registra una nueva transacción para el usuario autenticado.

- **URL:** `/api/usuari/transaccions`
- **Método:** `POST`
- **Autenticación:** Requerida (JWT Token)
- **Headers:**
  ```
  Content-Type: application/json
  Authorization: Bearer <token_jwt>
  ```
- **Cuerpo de la solicitud:**
  ```json
  {
    "tipus": "compra",
    "quantitat": -5,
    "preu": 5,
    "descripcio": "Compra de servei"
  }
  ```
- **Parámetros:**
  - `tipus`: (String) Tipo de transacción (valores permitidos: 'compra', 'us', 'reemborsament', 'ajust')
  - `quantitat`: (Number) Cantidad de créditos (negativo para gastos)
  - `preu`: (Number, opcional) Precio de la transacción
  - `descripcio`: (String, opcional) Descripción de la transacción

- **Respuesta exitosa (201):**
  ```json
  {
    "message": "Transacció creada correctament",
    "transaccio": {
      "tipus": "compra",
      "quantitat": -5,
      "saldo_final": 155,
      "preu": 5,
      "descripcio": "Compra de servei"
    }
  }
  ```

### Obtener Historial de Transacciones

Obtiene todas las transacciones del usuario autenticado.

- **URL:** `/api/usuari/transaccions`
- **Método:** `GET`
- **Autenticación:** Requerida (JWT Token)
- **Headers:**
  ```
  Authorization: Bearer <token_jwt>
  ```
- **Respuesta exitosa (200):**
  ```json
  {
    "message": "Transaccions obtingudes correctament",
    "transactions": [
      {
        "id": 123,
        "tipus": "compra",
        "quantitat": 10,
        "saldo_final": 160,
        "preu": 10,
        "descripcio": "Compra de 10 crèdits amb targeta",
        "data_transaccio": "2023-11-08T15:45:30.123Z"
      },
      {
        "id": 124,
        "tipus": "us",
        "quantitat": -5,
        "saldo_final": 155,
        "preu": null,
        "descripcio": "Us de servei",
        "data_transaccio": "2023-11-09T10:30:15.456Z"
      }
    ]
  }
  ```
## Gestión de Feedback

- **Veure valoracions**

  - **URL:** `/api/valoracions`
  - **Método:** `GET`
  - **Cuerpo de la solicitud:**

    ```json
    {
    }
    ```

- **Crear valoracions**

  - **URL:** `/api/valoracions/crear`
  - **Método:** `POST`
  - **Cuerpo de la solicitud:**

    ```json
    {
      "servei_id": 4,
      "puntuacio": 5,
      "comentari": "Servei excel·lent!"
    }
    ```

---

## Gestió de serveis

- **Donar d'alta un servei**

  - **URL:** `/api/serveis/crear`
  - **Método:** `POST`
  - **Cuerpo de la solicitud:**

    ```json
    {
      "nom": "Classe de Natació 3",
      "descripcio": "Sessió 3 de Natació a tope de power",
      "data": "2025-08-27 13:05:00",
      "ubicacio": "BCN",
      "preu": 3.50,
      "duracio_mins": 20,
      "capacitat": 5,
      "estat_id": 1,
      "tipus_servei_id": 1
    }
   ```

- **Modificar Servei**

  - **URL:** `/api/serveis/modificar/:id`
  - **Método:** `PUT`
  - **Cuerpo de la solicitud:**

    ```json
    {
      "nom": "Clase de spinning",
      "descripcio": "Sessió anaeróbica...",
      "data": "2022-11-27 18:05:00",
      "ubicacio": "Mi casa",
      "preu": 15.50,
      "duracio_mins": 37,
      "capacitat": 14,
      "estat_id": 1,
      "tipus_servei_id": 1
    }
    ```

---

## Gestió de reserves

- **Visualitzar reserves (Profesional)**

  - **URL:** `/api/reserves/visualitzar`
  - **Método:** `GET`
  - **Cuerpo de la solicitud:**

    ```json
    {

    }
    ```

- **Visualitzar reserves (Usuari)**

  - **URL:** `/api/reserves/visualitzar/jo`
  - **Método:** `GET`
  - **Cuerpo de la solicitud:**

    ```json
    {
      
    }
    ```

---

## Marketplace

- **Navegació catàleg**

  - **URL:** `/api/productes`
  - **Método:** `POST`
  - **Cuerpo de la solicitud:**

    ```json
    {
      "categories": [1, 2, 5] 
      // CategoriaID (1-6), suporta més d'una categoria
      // a l'hora, buit --> totes.
    }
    ```

---


## Estructura del proyecto

- `app.js`: Configuración principal del servidor Express.
- `controllers/authController.js`: Controladores para las rutas de autenticación.
- `controllers/subscripcionsController.js`: Controladores para las rutas de subscripciones.
- `controllers/userController.js`: Controladores para las rutas de usuario.
- `models/db.js`: Configuración de la conexión a la base de datos MariaDB.
- `routes/authRoutes.js`: Definición de las rutas de autenticación.
- `routes/subscriptonRoutes.js`: Definición de las rutas de subscripcion.
- `routes/userRoutes.js`: Definición de las rutas de usuario.

## Contribuir

Si deseas contribuir a este proyecto, por favor sigue los siguientes pasos:

1. Haz un fork del repositorio.
2. Crea una nueva rama (`git checkout -b feature/nueva-funcionalidad`).
3. Realiza tus cambios y haz commit (`git commit -am 'Añadir nueva funcionalidad'`).
4. Sube tus cambios a tu fork (`git push origin feature/nueva-funcionalidad`).
5. Abre un Pull Request.

## Licencia

Este proyecto está bajo la Licencia MIT. Consulta el archivo `LICENSE` para más detalles.
