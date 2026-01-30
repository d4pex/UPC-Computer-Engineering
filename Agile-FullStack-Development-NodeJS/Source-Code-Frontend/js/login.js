document.getElementById("loginForm").addEventListener("submit", async function(event) {
    event.preventDefault();

    const errorMessage = document.getElementById("error-message");
    errorMessage.innerHTML = "";

    const dni = document.getElementById("dni").value;
    const password = document.getElementById("password").value;

    // Validació del DNI
    if (!/^[0-9]{8}[A-Za-z]$/.test(dni)) {
        errorMessage.innerHTML = "Format de DNI incorrecte. Ha de tenir 8 números seguits d'una lletra.";
        return;
    }

    const data = { dni, contrasenya: password };

    try {
        const response = await fetch("http://localhost:3000/api/auth/login", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(data)
        });

        const result = await response.json();

        if (response.ok) {
            // Desa el token a localStorage
            localStorage.setItem("authToken", result.token); 

            errorMessage.style.color = "green";
            errorMessage.innerHTML = "Inici de sessió amb èxit. Redirigint...";
            
            setTimeout(() => window.location.href = "../html/dashboard.html", 2000); // Redirigir al dashboard
        } else {
            errorMessage.style.color = "red";
            errorMessage.innerHTML = result.message || "Error en l'inici de sessió. Torna-ho a intentar-ho.";
        }
    } catch (error) {
        errorMessage.style.color = "red";
        errorMessage.innerHTML = "Error de connexió amb el servidor.";
        console.error(error);
    }
});