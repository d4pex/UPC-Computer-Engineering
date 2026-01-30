// File: js/progresRepte.js

const API_BASE_URL = "http://localhost:3000";

document.addEventListener("DOMContentLoaded", async () => {
    const params = new URLSearchParams(window.location.search);
    const inscrId = params.get('inscr_id');
    const token = localStorage.getItem('authToken');

    if (!token || !inscrId) {
        window.location.href = 'login.html';
        return;
    }

    try {
        const url = `${API_BASE_URL}/api/inscripcions/${encodeURIComponent(inscrId)}/progres`;
        const resp = await fetch(url, {
            headers: {
                'Authorization': `Bearer ${token}`,
                'Accept': 'application/json'
            }
        });

        // Error 404
        if (resp.status === 404) {
            const err = await resp.json().catch(() => ({ message: 'No trobat.' }));
            return mostrarMissatge(err.message, 'red');
        }
        if (!resp.ok) {
            const err = await resp.json().catch(() => ({ message: `Error ${resp.status}` }));
            return mostrarMissatge(err.message, 'red');
        }

        // Rebem progress i objectiu
        const { progress, objectiu } = await resp.json();

        const cont = document.getElementById('circuit-progres');
        cont.innerHTML = "";

        // Mostrem text i circuit amb objectiu dinàmic
        mostrarProgresText(progress, objectiu);
        renderitzarCircuit(progress, objectiu);

    } catch (e) {
        console.error(e);
        mostrarMissatge("Error de comunicació", 'red');
    }
});

function mostrarProgresText(progress, objectiu) {
    const cont = document.getElementById('circuit-progres');
    const info = document.createElement('p');
    info.className = 'progres-info';
    info.textContent = `Progrés: ${progress} / ${objectiu}`;
    cont.appendChild(info);
}

function renderitzarCircuit(progress, objectiu) {
    const container = document.getElementById('circuit-progres');
    const wrapper = document.createElement('div');
    wrapper.className = 'circuit';

    for (let i = 1; i <= objectiu; i++) {
        // Cercle numerat
        const point = document.createElement('div');
        point.className = 'circuit-point';
        point.textContent = i;
        if (i <= progress) point.classList.add('completed');
        wrapper.appendChild(point);

        // Línia (excepte després de l'últim cercle)
        if (i < objectiu) {
            const line = document.createElement('div');
            line.className = 'circuit-line';
            if (i < progress) line.classList.add('completed');
            wrapper.appendChild(line);
        }
    }

    container.appendChild(wrapper);
}

function mostrarMissatge(text, color) {
    const msg = document.getElementById('missatge');
    msg.textContent = text;
    msg.style.color = color;
    msg.style.display = 'block';
}
