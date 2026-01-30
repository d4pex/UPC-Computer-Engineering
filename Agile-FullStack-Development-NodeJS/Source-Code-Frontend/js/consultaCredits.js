async function carregarCredits(token) {
    try {
        const response = await fetch("http://localhost:3000/api/usuari/credits", {
            method: "GET",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Content-Type": "application/json"
            }
        });

        if (!response.ok) {
            throw new Error(`Error en obtenir els crèdits: ${response.status}`);
        }

        const data = await response.json();
        const credits = data.credits || 0;

        const creditsSpan = document.getElementById("creditos");
        if (creditsSpan) {
            creditsSpan.textContent = credits;
        } else {
            console.warn("No s'ha trobat l'element amb ID 'creditos'");
        }

    } catch (error) {
        console.error("Error carregant crèdits:", error);
        showMessage("No s'han pogut carregar els crèdits.", "red");
    }
}

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

document.addEventListener("DOMContentLoaded", () => {
    const token = localStorage.getItem("authToken");

    if (!token) {
        window.location.href = "../html/login.html"; // Redirigeix si no hi ha sessió activa
        return;
    }

    carregarCredits(token);
});
