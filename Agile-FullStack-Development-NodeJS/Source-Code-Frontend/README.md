# Moute Frontend

Este proyecto es la interfaz de usuario de Mou-te+, desarrollada con HTML, CSS y JavaScript. Proporciona una experiencia intuitiva para la autenticación, gestión de cuentas y selección de planes. Se comunica con el backend a través de peticiones HTTP y gestiona dinámicamente la interfaz mediante JavaScript.

## Requisitos

Antes de comenzar, asegúrate de tener instalado lo siguiente:

- Un navegador web moderno (Chrome, Firefox, Edge)
- Un servidor local opcional para pruebas (como Live Server en VS Code)
- Git (opcional, para gestionar el código fuente)

## Instalación

1. Clona el repositorio:

   ```bash
   git clone https://github.com/tu-usuario/moute-frontend.git
   cd moute-frontend
   ```

## Uso

1 Inicia el servidor web:

   ```bash
   python3 -m http.server 80
   ```

2 Abre el navegador web y escribe:

   ```bash
   http://localhost
   ```

## Estructura del proyecto

- /css/estils.css: Archivo de estilos principal para la interfaz.
- /js/canviarContrasenya.js: Lógica para el cambio de contraseña.
- /js/esborrarCompte.js: Manejo de la eliminación de cuentas.
- /js/login.js: Validación y envío de datos para el inicio de sesión.
- /js/logout.js: Gestión del cierre de sesión.
- /js/perfil.js: Funcionalidad para la página de perfil.
- /js/registre.js: Manejo del registro de usuarios.
- /js/seleccioPla.js: Lógica para la selección de planes.
- /html/canviarContrasenya.html: Página para cambiar la contraseña.
- /html/dashboard.html: Interfaz del panel de usuario.
- /html/esborrarCompte.html: Página para eliminar la cuenta.
- /html/login.html: Formulario de inicio de sesión.
- /html/logout.html: Página de cierre de sesión.
- /html/perfil.html: Página de perfil del usuario.
- /html/registre.html: Formulario de registro.
- /html/seleccioPla.html: Página para la selección de planes.
- /img/: Carpeta para imágenes del proyecto.
- index.html: Página principal del sitio.
- README.md: Documentación del proyecto.s

## Contribuir

Si deseas contribuir a este proyecto, por favor sigue los siguientes pasos:

1. Haz un fork del repositorio.
2. Crea una nueva rama (`git checkout -b {nueva rama}`).
3. Realiza tus cambios y haz commit (`git commit -m 'Añadir nueva funcionalidad'`).
4. Sube tus cambios a tu fork (`git push origin {nueva rama}`).
5. Abre un Pull Request.
