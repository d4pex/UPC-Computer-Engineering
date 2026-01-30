document.addEventListener("DOMContentLoaded", () => {
    const token = localStorage.getItem("authToken"); // Suposant que el token s'emmagatzema aquí

    if (!token) {
        window.location.href = "../html/login.html"; // Redirigeix si no hi ha sessió activa
        return;
    }

    const form = document.getElementById("deleteAccountForm");

    form.addEventListener("submit", async (event) => {
        event.preventDefault();
        
        const password = document.getElementById("password").value;  // Capturem la contrasenya
    
        try {
            const response = await fetch("http://localhost:3000/api/usuari/eliminar", {
                method: "POST",
                headers: {
                    "Authorization": `Bearer ${token}`,
                    "Content-Type": "application/json"
                },
                body: JSON.stringify({ contrasenya: password })
            });
    
            if (response.ok) {
                localStorage.removeItem("authToken");
                showMessage("Compte eliminat correctament.", "green");
                setTimeout(() => { window.location.href = "../index.html"; }, 1500);
            } else {
                throw new Error("Contrasenya incorrecta.");
            }
        } catch (error) {
            console.error("Error:", error);
            showMessage("No s'ha pogut eliminar el compte.", "red");
        }
    });
});

function showMessage(message, type) {
    const messageDiv = document.createElement("div");
    messageDiv.classList.add("message-box");
    messageDiv.style.display = "block";
    messageDiv.textContent = message;
    messageDiv.style.backgroundColor = type === "green" ? "#d4edda" : "#f8d7da";
    messageDiv.style.color = type === "green" ? "#155724" : "#721c24";
    messageDiv.style.border = type === "green" ? "1px solid #c3e6cb" : "1px solid #f5c6cb";
    document.body.prepend(messageDiv);
}
