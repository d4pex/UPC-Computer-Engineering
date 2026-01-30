document.getElementById("confirm-logout").addEventListener("click", async function () {
    const token = localStorage.getItem("authToken");

    if (!token) {
        alert("No hay sesión activa.");
        window.location.href = "../html/login.html";
        return;
    }
    const apiUrl = `http://localhost:3000/api/usuari/logout`;
    try {
        const response = await fetch(apiUrl, {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${token}`
            }
        });

        if (response.ok) {
            // Eliminar el token del localStorage
            localStorage.removeItem("authToken");

            alert("Sesión cerrada exitosamente.");
            window.location.href = "../html/login.html"; // Redirigir al login
        } else {
            alert("Error al cerrar sesión. Inténtalo de nuevo.");
        }
    } catch (error) {
        console.error("Error al cerrar sesión:", error);
        alert("Error de conexión con el servidor.");
    }
});

// Manejo del botón "Cancelar"
document.getElementById("cancel-logout").addEventListener("click", function () {
    window.location.href = "../html/dashboard.html";
});