let puntuacioSeleccionada = 0;
let serveiSeleccionatId = null;

console.log("Script de valoracions cargado");

// Estrellas: seleccionar puntuación
document.querySelectorAll('.estrella-filtre').forEach(estrella => {
    estrella.addEventListener('click', () => {
        puntuacioSeleccionada = parseInt(estrella.dataset.valor);

        document.querySelectorAll('.estrella-filtre').forEach(e => {
            const valor = parseInt(e.dataset.valor);
            e.classList.toggle('seleccionada', valor <= puntuacioSeleccionada);
        });
    });
});

// Obtener y mostrar reservas pasadas
async function cargarReservasPasadas() {
    try {
        const token = localStorage.getItem("authToken");
        console.log("Token:", token);

        const response = await fetch("http://localhost:3000/api/reserves/visualitzar/jo", {
            method: "GET",
            headers: {
                "Content-Type": "application/json",
                Authorization: `Bearer ${token}`,
            }
        });

        const result = await response.json();
        console.log("Reserves rebudes:", result);

        const ahora = new Date();
        const unaSemanaAtras = new Date();
        unaSemanaAtras.setDate(unaSemanaAtras.getDate() - 7);

        const serviciosMostrados = new Set();
        const contenedor = document.getElementById('lista-reservas');
        contenedor.innerHTML = '';

        result.forEach(reserva => {
            if (!reserva.servei_nom || !reserva.data_reserva) {
                console.warn("Reserva sin servei_nom o data_reserva:", reserva);
                return;
            }
        
            const fechaReserva = new Date(reserva.data_reserva);
            console.log("Fecha de reserva:", fechaReserva);

            // Validar fecha válida
            if (isNaN(fechaReserva)) {
                console.warn("Fecha inválida para la reserva:", reserva.data_reserva);
                return;
            }

            const idServei = reserva.servei_id;

            // Mostrar solo reservas pasadas en la última semana y sin repetir servicios
            if (fechaReserva < ahora && fechaReserva >= unaSemanaAtras && !serviciosMostrados.has(idServei)) {
                serviciosMostrados.add(idServei);

                // Formatear la fecha para mostrarla mejor
                const fechaFormateada = fechaReserva.toLocaleString('es-ES', {
                    day: '2-digit',
                    month: '2-digit',
                    year: 'numeric',
                    hour: '2-digit',
                    minute: '2-digit',
                    hour12: false
                });

                const div = document.createElement('div');
                div.className = 'reserva-item';
                div.textContent = `Servei: ${reserva.servei_nom} - Data: ${fechaFormateada}`;
                div.style.cursor = 'pointer';
                div.addEventListener('click', () => {
                    serveiSeleccionatId = idServei;
                    document.getElementById('titulo').value = reserva.servei_nom;
                    document.getElementById('valoracions-container').style.display = 'block';
                    document.getElementById('lista-reservas').style.display = 'none'; // <-- ocultar reservas
                    document.getElementById('titulo-valoracions').style.display = 'none'; // Ocultar título
                });

                contenedor.appendChild(div);
            }
        });

        console.log("Servicios mostrados:", serviciosMostrados);
        if (serviciosMostrados.size === 0) {
            contenedor.innerHTML = '<p>No tens reserves passades per valorar.</p>';
        }

    } catch (error) {
        console.error("Error al cargar reservas:", error);
    }
}

cargarReservasPasadas();

// Enviar valoración
document.getElementById('valoracioForm').addEventListener('submit', async function(e) {
    e.preventDefault();

    const comentario = document.getElementById('comentario').value;

    if (!serveiSeleccionatId) {
        alert("Si us plau, selecciona una reserva per valorar.");
        return;
    }

    if (puntuacioSeleccionada === 0) {
        alert("Si us plau, selecciona una puntuació.");
        return;
    }

    const data = {
        servei_id: serveiSeleccionatId,
        puntuacio: puntuacioSeleccionada,
        comentari: comentario,
    };

    try {
        const token = localStorage.getItem("authToken");
        const response = await fetch("http://localhost:3000/api/valoracions/crear", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                Authorization: `Bearer ${token}`,
            },
            body: JSON.stringify(data),
        });

        const result = await response.json();
console.log("Resposta del servidor:", result);

        if (result.message === "Ja has valorat aquest servei") {
        alert("Ja has valorat aquest servei abans.");
        } else {
        alert("Valoració enviada correctament!");
        }


        // Reiniciar formulario
        document.getElementById('valoracioForm').reset();
        document.querySelectorAll('.estrella-filtre').forEach(e => e.classList.remove('seleccionada'));
        puntuacioSeleccionada = 0;
        serveiSeleccionatId = null;

    } catch (error) {
        alert("Hubo un error al conectar con el servidor.");
        console.error(error);
    }
});
