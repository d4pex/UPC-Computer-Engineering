// File: js/reptesInscrits.js

const API_BASE_URL = "http://localhost:3000";

document.addEventListener("DOMContentLoaded", async () => {
    const token = localStorage.getItem("authToken");

    if (!token) {
        alert("No s'ha trobat cap token. Si us plau, inicia sessió.");
        window.location.href = "login.html";
        return;
    }

    await carregarReptesInscrits(token);
});

async function carregarReptesInscrits(token) {
    try {
        const response = await fetch(`${API_BASE_URL}/api/inscripcions`, {
            headers: {
                "Authorization": `Bearer ${token}`,
                "Accept": "application/json"
            }
        });

        if (!response.ok) throw new Error(`Error ${response.status} carregant reptes inscrits.`);

        const reptes = await response.json();
        renderitzarReptesInscrits(reptes);
    } catch (error) {
        mostrarMissatge(error.message || "Error en carregar els reptes inscrits.", "red");
    }
}

function renderitzarReptesInscrits(reptes) {
    const container = document.getElementById("llista-reptes-inscrits");
    container.innerHTML = "";

    if (!Array.isArray(reptes) || reptes.length === 0) {
        container.innerHTML = "<p>No estàs inscrit en cap repte.</p>";
        return;
    }

    reptes.forEach(item => {
        const inscripcio = item;
        const repte = inscripcio.repte ?? inscripcio;
        const repteId = inscripcio.repte?.id ?? inscripcio.id;
        const completat = inscripcio.completat ?? inscripcio.estat === 'completat';

        const div = document.createElement("div");
        div.className = "pack";

        let accionsHTML = "";

        if (completat) {
            accionsHTML = `
                <div class="repte-completat">
                    ✅ <strong>Repte completat!</strong>
                </div>
            `;
        } else {
            accionsHTML = `
                <div class="buttons-container">
                    <button class="button btn-progres ml-4" data-id="${inscripcio.id}">
                        Progrés Repte
                    </button>
                    <button class="button btn-completar" data-id="${repteId}">
                        Marcar com a completat
                    </button>
                    <button class="button btn-esborrar ml-4" data-id="${repteId}">
                        Cancel·lar inscripció
                    </button>
                </div>
            `;
        }

        div.innerHTML = `
            <img src="../img/credits-logo.png" alt="Repte">
            <h3>${repte.nom}</h3>
            <p>${repte.descripcio}</p>
            <p><strong>Preu:</strong> ${repte.preu} crèdits</p>
            <p><strong>Recompensa:</strong> ${repte.credits_recompensa} crèdits</p>
            <p><strong>Data límit:</strong> ${repte.data_fi ? new Date(repte.data_fi).toLocaleDateString() : '—'}</p>
            ${accionsHTML}
        `;

        container.appendChild(div);
    });

    document.querySelectorAll(".btn-completar")
        .forEach(btn => btn.addEventListener("click", marcarCompletat));

    document.querySelectorAll(".btn-esborrar")
        .forEach(btn => btn.addEventListener("click", cancelarInscripcio));

    document.querySelectorAll(".btn-progres")
        .forEach(btn => btn.addEventListener("click", veureProgres));
}

function veureProgres(event) {
    const inscrId = event.target.dataset.id;  // ahora es la inscripció.id
    window.location.href = `progresRepte.html?inscr_id=${inscrId}`;
}


async function marcarCompletat(event) {
    const repteId = event.target.dataset.id;
    const token = localStorage.getItem("authToken");

    try {
        const response = await fetch(`${API_BASE_URL}/api/inscripcions/${repteId}/completar`, {
            method: "PUT",
            headers: { "Authorization": `Bearer ${token}` }
        });

        if (response.ok) {
            mostrarMissatge("Repte marcat com a completat!", "green");
            await carregarReptesInscrits(token);
            await carregarCredits(token);
        } else {
            const { message } = await response.json().catch(() => ({}));
            mostrarMissatge(message || "Error en marcar el repte com a completat.", "red");
        }
    } catch {
        mostrarMissatge("Error en connectar amb el servidor.", "red");
    }
}

async function cancelarInscripcio(event) {
    const repteId = event.target.dataset.id;
    const token = localStorage.getItem("authToken");

    if (!confirm("Estàs segur que vols cancel·lar la teva inscripció a aquest repte?")) return;

    try {
        const response = await fetch(`${API_BASE_URL}/api/inscripcions/${repteId}`, {
            method: "DELETE",
            headers: { "Authorization": `Bearer ${token}` }
        });

        if (response.ok) {
            mostrarMissatge("Inscripció cancel·lada correctament.", "green");
            await carregarReptesInscrits(token);
        } else if (response.status === 404) {
            mostrarMissatge("No estaves inscrit en aquest repte.", "red");
        } else {
            mostrarMissatge("Error en cancel·lar la inscripció.", "red");
        }
    } catch {
        mostrarMissatge("Error en connectar amb el servidor.", "red");
    }
}

function mostrarMissatge(text, color) {
    const missatge = document.getElementById("missatge");
    missatge.textContent = text;
    missatge.style.color = color;
    missatge.style.display = "block";

    const rect = missatge.getBoundingClientRect();
    const scrollTop = window.pageYOffset || document.documentElement.scrollTop;
    const posMissatge = rect.top + scrollTop;

    window.scrollTo({ top: posMissatge, behavior: 'smooth' });

    setTimeout(() => { missatge.style.display = "none"; }, 4000);
}


async function marcarCompletat(event) {
    const repteId = event.target.dataset.id;
    const token = localStorage.getItem("authToken");

    try {
        const response = await fetch(`${API_BASE_URL}/api/inscripcions/${repteId}/completar`, {
            method: "PUT",
            headers: { "Authorization": `Bearer ${token}` }
        });

        if (response.ok) {
            mostrarMissatge("Repte marcat com a completat!", "green");
            await carregarReptesInscrits(token);
        } else {
            const { message } = await response.json().catch(() => ({}));
            mostrarMissatge(message || "Error en marcar el repte com a completat.", "red");
        }
    } catch {
        mostrarMissatge("Error en connectar amb el servidor.", "red");
    }
}

async function cancelarInscripcio(event) {
    const repteId = event.target.dataset.id;
    const token = localStorage.getItem("authToken");

    if (!confirm("Estàs segur que vols cancel·lar la teva inscripció a aquest repte?")) return;

    try {
        const response = await fetch(`${API_BASE_URL}/api/inscripcions/${repteId}`, {
            method: "DELETE",
            headers: { "Authorization": `Bearer ${token}` }
        });

        if (response.ok) {
            mostrarMissatge("Inscripció cancel·lada correctament.", "green");
            await carregarReptesInscrits(token);
        } else if (response.status === 404) {
            mostrarMissatge("No estaves inscrit en aquest repte.", "red");
        } else {
            mostrarMissatge("Error en cancel·lar la inscripció.", "red");
        }
    } catch {
        mostrarMissatge("Error en connectar amb el servidor.", "red");
    }
}

function mostrarMissatge(text, color) {
    const missatge = document.getElementById("missatge");
    missatge.textContent = text;
    missatge.style.color = color;
    missatge.style.display = "block";

    const rect = missatge.getBoundingClientRect();
    const scrollTop = window.pageYOffset || document.documentElement.scrollTop;
    const posMissatge = rect.top + scrollTop;

    window.scrollTo({ top: posMissatge, behavior: 'smooth' });

    setTimeout(() => { missatge.style.display = "none"; }, 4000);
}
