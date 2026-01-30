const API_BASE_URL = "http://localhost:3000";

document.addEventListener("DOMContentLoaded", async () => {
    const token = localStorage.getItem("authToken");

    if (!token) {
        alert("No s'ha trobat cap token. Si us plau, inicia sessió.");
        window.location.href = "login.html";
        return;
    }

    try {
        const response = await fetch(`${API_BASE_URL}/api/reptes`, {
            headers: {
                "Authorization": `Bearer ${token}`
            }
        });

        if (!response.ok) throw new Error("Error carregant reptes.");

        const reptes = await response.json();
        renderitzarReptes(reptes);
    } catch (error) {
        mostrarMissatge("Error en carregar els reptes.", "red");
        console.error(error);
    }
});

function renderitzarReptes(reptes) {
    const container = document.getElementById("llista-reptes");
    container.innerHTML = "";

    if (reptes.length === 0) {
        container.innerHTML = "<p>No hi ha reptes disponibles.</p>";
        return;
    }

    reptes.forEach(repte => {
        const div = document.createElement("div");
        div.className = "pack";

        div.innerHTML = `
            <img src="../img/credits-logo.png" alt="Repte" />
            <h3>${repte.nom}</h3>
            <p>${repte.descripcio}</p>
            <p><strong>Preu:</strong> ${repte.preu} crèdits</p>
            <p><strong>Recompensa:</strong> ${repte.credits_recompensa} crèdits</p>
            <p><strong>Data límit:</strong> ${repte.data_fi}</p>
            <p><strong>Objectiu:</strong> ${repte.objectiu}</p>
            <button class="button btn-inscriure" data-id="${repte.id}">Inscriu-te</button>
        `;

        container.appendChild(div);
    });

    // Añadir eventos click a los botones de inscripción
    document.querySelectorAll(".btn-inscriure").forEach(btn => {
        btn.addEventListener("click", inscriureARepte);
    });
}

async function inscriureARepte(event) {
    const repteId = event.target.dataset.id;
    const token = localStorage.getItem("authToken");

    try {
        const response = await fetch(`${API_BASE_URL}/api/inscripcions`, {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${token}`
            },
            body: JSON.stringify({ repte_id: parseInt(repteId) })
        });

        if (response.status === 201) {
            mostrarMissatge("Inscripció correcta al repte!", "green");
        } else if (response.status === 409) {
            mostrarMissatge("Ja estàs inscrit en aquest repte.", "orange");
        } else if (response.status === 402) {
            mostrarMissatge("No tens crèdits suficients per inscriure't.", "red");
        } else {
            const text = await response.text();
            console.error("Error inesperat:", text);
            mostrarMissatge("S'ha produït un error inesperat.", "red");
        }
    } catch (error) {
        console.error("Error en la inscripció:", error);
        mostrarMissatge("Error en connectar amb el servidor.", "red");
    }
}

function mostrarMissatge(text, color) {
    const missatge = document.getElementById("missatge");
    missatge.textContent = text;
    missatge.style.display = "block";

    if (color === "green") {
        missatge.style.color = "#155724";
        missatge.style.backgroundColor = "#d4edda";
        missatge.style.border = "1px solid #c3e6cb";
    } else if (color === "red") {
        missatge.style.color = "#721c24";
        missatge.style.backgroundColor = "#f8d7da";
        missatge.style.border = "1px solid #f5c6cb";
    } else if (color === "orange") {
        missatge.style.color = "#856404";
        missatge.style.backgroundColor = "#fff3cd";
        missatge.style.border = "1px solid #ffeeba";
    } else {
        missatge.style.color = "";
        missatge.style.backgroundColor = "";
        missatge.style.border = "";
    }

    setTimeout(() => {
        missatge.style.display = "none";
    }, 4000);
}
