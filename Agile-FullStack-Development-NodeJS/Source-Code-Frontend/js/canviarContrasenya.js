document.getElementById("loginForm").addEventListener("submit", async function (event) {
    event.preventDefault();
    const passwordC = document.getElementById("passwordCurrent").value;
    const password = document.getElementById("password").value;
    const password2 = document.getElementById("password2").value;

    // Comprovar que les contraenyes coincideixen
    if (password !== password2) {
        alert("Las contraseñas no coinciden. Por favor, inténtalo de nuevo.");
        return;
    }

    // Comprovar que les contraenyes tenen un format correcte
    const passwordRegex = /^(?=.*[A-Za-z])(?=.*\d)(?=.*[@$!%*?&#])[A-Za-z\d@$!%*?&#]{8,}$/;
    if (!passwordRegex.test(password)) {
        alert("La contraseña debe tener al menos 8 caracteres, una letra, un número y un carácter especial.");
        return;
    }

    // Preparar les dades per la solicitud
    const data = {
        oldPass: passwordC,
        newPass: password,
        newPassConf: password2
    };

    console.log("Datos a enviar:", data);

    // Obtenim el token de localStorage
    const token = localStorage.getItem("authToken");
    console.log("Token recuperado:", token);

    try {

        if (!token) {
            alert("");
            return;
        }

        // Fem la solicitud per canviar la contrasenya
        const response = await fetch("http://localhost:3000/api/usuari/canviar-contrasenya", {
            method: "PUT",
            headers: {
              "Content-Type": "application/json",
              Authorization: `Bearer ${token}`,
            },
            body: JSON.stringify(data),
          });

        // Depurar la respuesta del servidor
        const result = await response.json();

        if (response.ok) {
            alert("Contrasenya canviada exitosament");
            window.location.href = "../html/perfil.html";  // Redirigir a la página principal
        } else {
            alert("Error al canviar la contrasenya: " + (result.message || "Torna a provar."));
        }
    } catch (error) {
        console.error("Hubo un error en la conexión:", error);
        alert("Hubo un error al conectar con el servidor.");
    }
});


// Función para mostrar notificaciones
function showNotification(message, type = "success") {
    const notification = document.getElementById("notification");
    const notificationMessage = document.getElementById("notification-message");

    notification.classList.remove("show");
    notification.classList.add("hide");

    setTimeout(() => {
        notificationMessage.textContent = message;

        // Definir tipo de notificación
        notification.classList.remove("hide");
        notification.classList.add("show");

        if (type === "error") {
            notification.classList.add("notification-error");
        } else {
            notification.classList.remove("notification-error");
        }

        // Ocultar notificación después de 3 segundos
        setTimeout(() => {
            notification.classList.remove("show");
            notification.classList.add("hide");
        }, 3000);
    }, 100);
}