document.addEventListener("DOMContentLoaded", () => {
    const listaReserves = document.getElementById("llista-reserves");
    const filtreServei = document.getElementById("filtre-servei");
    const filtreData = document.getElementById("filtre-data");
    const botoFiltrar = document.getElementById("boto-filtrar");
    const botoReset = document.getElementById("boto-reset");
    const missatgeBuit = document.getElementById("missatge-buit");

    if (!listaReserves) return;

    const token = localStorage.getItem("authToken");

    let reserves = [];

    const obtenirServeiDetallat = async (nomServei) => {
        const response = await fetch(`http://localhost:3000/api/serveis?nom=${encodeURIComponent(nomServei)}`);
        if (!response.ok) throw new Error("Error al obtenir el servei");
        const data = await response.json();
        return data.serveis?.[0]; // Retornem el primer servei trobat amb aquest nom
    };

    const renderReserves = async () => {
        listaReserves.innerHTML = "";

        const filtreServeiText = filtreServei.value.toLowerCase();
        const filtreDataValue = filtreData.value;
        
        const filtrades = reserves.filter(reserva => {
            const coincideixServei = reserva.servei_nom.toLowerCase().includes(filtreServeiText);
            const coincideixData = filtreDataValue ? new Date(reserva.data_reserva).toISOString().split("T")[0] === filtreDataValue : true;
            const estatValid = reserva.estat_id !== 3; // Filtra les que no són estat 2
            return coincideixServei && coincideixData && estatValid;
        });
        console.log("Reserves filtrades:", filtrades);

        if (filtrades.length === 0) {
            missatgeBuit.style.display = "block";
        } else {
            missatgeBuit.style.display = "none";
        }
        
        for (const reserva of filtrades) {
            try {
                const serveiDetallat = await obtenirServeiDetallat(reserva.servei_nom);
                const listItem = document.createElement("li");
                listItem.classList.add("reserva-item");

                const fechaReserva = new Date(reserva.data_reserva);
                const fechaFormateada = fechaReserva.toLocaleString("ca-ES", {
                    weekday: "long", year: "numeric", month: "long", day: "numeric", hour: "2-digit", minute: "2-digit"
                });

                let ubicacio = "Desconeguda";
                let horaServei = "Desconeguda";
                
                if (serveiDetallat) {
                    ubicacio = serveiDetallat.ubicacio || "Desconeguda";
                    const dataHoraServei = new Date(serveiDetallat.data);
                    horaServei = dataHoraServei.toLocaleTimeString("ca-ES", { hour: "2-digit", minute: "2-digit" });
                }
                
                const contenido = `
                    <p><strong>Data de la Reserva:</strong> ${fechaFormateada}</p>
                    <p><strong>Servei:</strong> ${reserva.servei_nom}</p>
                    <p><strong>Ubicació:</strong> ${ubicacio}</p>
                    <p><strong>Hora del servei:</strong> ${horaServei}</p>
                    <button class="btn" onclick="location.href='cancelacioReserva.html?reservaId=${reserva.reserva_id}'">Cancel·lar Reserva</button>
                `;

                listItem.innerHTML = contenido;
                listaReserves.appendChild(listItem);
            } catch (error) {
                console.error("Error obtenint el servei detallat:", error);
            }
        }
    };

    try {
        fetch("http://localhost:3000/api/reserves/visualitzar/jo", {
            method: "GET",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Content-Type": "application/json"
            }
        })
        .then(response => {
            if (!response.ok) throw new Error("Error al obtenir les reserves");
            return response.json();
        })
        .then(data => {
            if (data && Array.isArray(data) && data.length > 0) {
                reserves = data;
                console.log("Reserves carregades:", reserves);
                renderReserves();
            } else {
                listaReserves.innerHTML = "<p>Encara no has fet cap reserva.</p>";
            }
        })
        .catch(error => {
            console.error("Error al carregar les reserves:", error);
            listaReserves.innerHTML = "<p>Error al carregar les reserves. Si us plau, intenta-ho més tard.</p>";
        });

        filtreServei.addEventListener("input", () => {
            renderReserves();
            botoReset.disabled = false;
        });

        filtreData.addEventListener("change", () => {
            renderReserves();
            botoReset.disabled = false;
        });

        botoReset.addEventListener("click", () => {
            filtreServei.value = "";
            filtreData.value = "";
            botoReset.disabled = true;
            renderReserves();
        });

    } catch (err) {
        console.error("Error carregant les reserves:", err);
    }
});
