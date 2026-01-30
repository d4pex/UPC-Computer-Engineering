document.addEventListener("DOMContentLoaded", () => {
    const listaReserves = document.getElementById("llista-reserves");
    const filtreServei = document.getElementById("filtre-servei");
    const filtreData = document.getElementById("filtre-data");
    const botoFiltrar = document.getElementById("boto-filtrar");
    const botoReset = document.getElementById("boto-reset");
    const missatgeBuit = document.getElementById("missatge-buit");

    if (!listaReserves) return;

    const token = localStorage.getItem("authToken");

    if (!token) {
        window.location.href = "../html/login.html";
        return;
    }

    // Decodificar el token JWT para obtener el rol
    const decodeToken = (token) => {
        const base64Url = token.split('.')[1];
        const base64 = base64Url.replace(/-/g, '+').replace(/_/g, '/');
        const jsonPayload = decodeURIComponent(atob(base64).split('').map(function(c) {
            return '%' + ('00' + c.charCodeAt(0).toString(16)).slice(-2);
        }).join(''));
        return JSON.parse(jsonPayload);
    };

    const decodedToken = decodeToken(token);
    console.log("Decoded Token:", decodedToken);
    const rol = decodedToken.rol; // Asegúrate de que 'rol_id' es el campo correcto
    console.log("Rol ID:", rol);

    // Verificar si el rol_id es diferente a 3 y redirigir si es necesario
    if (rol === 1) {
        window.location.href = "../html/veureReservesUsuari.html";
        return;
    }

    else if (rol !== 3) {
        // Limpiar el contenido principal si existe
        document.body.innerHTML = "";
    
        // Crear mensaje
        const missatge = document.createElement("div");
        missatge.textContent = "Aquest compte no té el rol de professional";
        missatge.style.fontSize = "1.5rem";
        missatge.style.marginBottom = "20px";
        missatge.style.textAlign = "center";
    
        // Crear botón
        const boto = document.createElement("button");
        boto.textContent = "Tornar a l'inici";
        boto.style.padding = "10px 20px";
        boto.style.fontSize = "1rem";
        boto.style.cursor = "pointer";
        boto.style.display = "block";
        boto.style.margin = "0 auto";
        boto.onclick = () => {
            window.location.href = "../html/dashboard.html";
        };
    
        // Agregar al body
        document.body.style.display = "flex";
        document.body.style.flexDirection = "column";
        document.body.style.justifyContent = "center";
        document.body.style.alignItems = "center";
        document.body.style.height = "100vh";
        document.body.appendChild(missatge);
        document.body.appendChild(boto);
    
        return;
    }

    let reserves = [];

    // Filtrar reservas
    const renderReserves = () => {
        listaReserves.innerHTML = "";

        const filtreServeiText = filtreServei.value.toLowerCase();
        const filtreDataValue = filtreData.value;

        const filtrades = reserves.filter(reserva => {
            const coincideixServei = reserva.servei_nom.toLowerCase().includes(filtreServeiText);
            const coincideixData = filtreDataValue ? new Date(reserva.data_reserva).toISOString().split("T")[0] === filtreDataValue : true;
            return coincideixServei && coincideixData;
        });

        if (filtrades.length === 0) {
            missatgeBuit.style.display = "block";
        } else {
            missatgeBuit.style.display = "none";
        }

        filtrades.forEach(reserva => {
            const listItem = document.createElement("li");
            listItem.classList.add("reserva-item");

            const fechaReserva = new Date(reserva.data_reserva);
            const fechaFormateada = fechaReserva.toLocaleString("ca-ES", {
                weekday: "long", year: "numeric", month: "long", day: "numeric", hour: "2-digit", minute: "2-digit"
            });

            const contenido = `
                <p><strong>Data de la Reserva:</strong> ${fechaFormateada}</p>
                <p><strong>Usuari:</strong> ${reserva.usuari_nom}</p>
                <p><strong>Email:</strong> ${reserva.usuari_mail}</p>
                <p><strong>Servei:</strong> ${reserva.servei_nom}</p>
            `;
            
            listItem.innerHTML = contenido;
            listaReserves.appendChild(listItem);
        });
    };

    try {
        fetch("http://localhost:3000/api/reserves/visualitzar", {
            method: "GET",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Content-Type": "application/json"
            }
        })
        .then(response => {
            if (!response.ok) throw new Error("Error al obtener las reservas");
            return response.json();
        })
        .then(data => {
            if (data && Array.isArray(data) && data.length > 0) {
                reserves = data;
                renderReserves();
            } else {
                listaReserves.innerHTML = "<p>No hi ha reserves disponibles.</p>";
            }
        })
        .catch(error => {
            console.error("Error al cargar las reservas:", error);
            listaReserves.innerHTML = "<p>Error al carregar les reserves. Si us plau, intenta-ho més tard.</p>";
        });

        // Filtrar por nombre de servicio
        filtreServei.addEventListener("input", () => {
            renderReserves();
            botoReset.disabled = false; // Habilitar el botón de reset si hay filtros
        });

        // Filtrar por fecha
        filtreData.addEventListener("change", () => {
            renderReserves();
            botoReset.disabled = false; // Habilitar el botón de reset si hay filtros
        });

        // Botón de reset
        botoReset.addEventListener("click", () => {
            // Limpiar los campos de filtro
            filtreServei.value = "";
            filtreData.value = "";

            // Deshabilitar el botón de reset
            botoReset.disabled = true;

            // Mostrar todas las reservas
            renderReserves();
        });

    } catch (err) {
        console.error("Error carregant les reserves:", err);
    }
});
