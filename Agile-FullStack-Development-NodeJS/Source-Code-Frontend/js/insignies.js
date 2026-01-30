document.addEventListener("DOMContentLoaded", async () => {
    const token = localStorage.getItem("authToken");

    if (!token) {
        window.location.href = "../html/login.html";
        return;
    }

    try {
        const response = await fetch("http://localhost:3000/api/inscripcions/badges", {
            method: "GET",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Content-Type": "application/json"
            }
        });

        if (!response.ok) {
            throw new Error("Error en obtenir les insígnies");
        }

        const badges = await response.json();
        mostrarInsignies(badges);
    } catch (error) {
        console.error("Error carregant insígnies:", error);
        mostrarMissatge("No s'han pogut carregar les insígnies.", "red");
    }
});

function mostrarInsignies(badges) {
    const container = document.getElementById("insignies-container");
    container.innerHTML = "";

    if (badges.length === 0) {
        container.textContent = "Encara no tens cap insígnia 😢";
        return;
    }

    // Mapeo de nombres de insignias a imágenes
    const iconMapping = {
        "Reto Gratuito": "../img/medalla.png",
        // Añade aquí otras insignias con sus rutas si quieres
    };

    badges.forEach(badge => {
        const card = document.createElement("div");
        card.classList.add("badge-card");

        const imgSrc = iconMapping[badge.nom] || "../img/medalla.png"; // fallback imagen genérica

        card.innerHTML = `
            <img src="${imgSrc}" alt="Insígnia ${badge.nom}" class="badge-icon" />
            <h3>${badge.nom}</h3>
            <p>${badge.descripcio}</p>
            <p><strong>Objectiu:</strong> ${badge.objectiu}</p>
            <p><strong>Crèdits recompensa:</strong> ${badge.credits_recompensa}</p>
        `;

        container.appendChild(card);
    });
}

function mostrarMissatge(missatge, tipus) {
    const messageDiv = document.createElement("div");
    messageDiv.classList.add("message-box");
    messageDiv.style.display = "block";
    messageDiv.textContent = missatge;
    messageDiv.style.backgroundColor = tipus === "green" ? "#d4edda" : "#f8d7da";
    messageDiv.style.color = tipus === "green" ? "#155724" : "#721c24";
    messageDiv.style.border = tipus === "green" ? "1px solid #c3e6cb" : "1px solid #f5c6cb";
    document.body.prepend(messageDiv);
}
