document.addEventListener("DOMContentLoaded", async () => {
    const token = localStorage.getItem("authToken");

    if (!token) {
        window.location.href = "../html/login.html";
        return;
    }

    const user = decodeToken(token);
    console.log("Usuario", user);
    if (!user || user.rol !== 3) {
        document.body.innerHTML = "";

        const missatge = document.createElement("div");
        missatge.textContent = "Aquest compte no té el rol de professional";
        missatge.style.fontSize = "1.5rem";
        missatge.style.marginBottom = "20px";
        missatge.style.textAlign = "center";

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

        document.body.style.display = "flex";
        document.body.style.flexDirection = "column";
        document.body.style.justifyContent = "center";
        document.body.style.alignItems = "center";
        document.body.style.height = "100vh";

        document.body.appendChild(missatge);
        document.body.appendChild(boto);

        return;
    }

    const buscarServeiBtn = document.getElementById("buscarServeiBtn");
    const form = document.getElementById("modificarServeiForm");

    const buscarServeiDiv = document.getElementById("buscarServeiDiv");
    const serveisList = document.getElementById("serveisList");
    const serveisEncontratsDiv = document.getElementById("serveisEncontrats");
    let serveiId = null;
    let estatId = null;

    buscarServeiBtn.addEventListener("click", async (event) => {
        event.preventDefault();

        const serveiTitol = document.getElementById("serveiTitol").value;
        if (!serveiTitol) {
            alert("El títol del servei no pot estar buit.");
            return;
        }

        const serveis = await carregarServeis(serveiTitol, token, user.userId);
        console.log("Servicios cargados:", serveis.length);
        if (serveis.length === 0) {
            alert("No s'han trobat serveis amb aquest títol.");
            return;
        }

        serveisList.innerHTML = "";

        serveis.forEach(servei => {
            const li = document.createElement("li");
            li.innerHTML = `
                <strong>${servei.nom}</strong><br>
                Descripció: ${servei.descripcio} <br>
                Preu: ${servei.preu} € <br>
                Duració: ${servei.duracio_mins} min <br>
                Ubicació: ${servei.ubicacio}
            `;
            li.addEventListener("click", () => seleccionarServei(servei));
            serveisList.appendChild(li);
        });

        buscarServeiDiv.style.display = "none";
        serveisEncontratsDiv.style.display = "block";
    });

    async function carregarServeis(serveiTitol, token, userId) {
        try {
            const response = await fetch(`http://localhost:3000/api/serveis?nom=${serveiTitol}`, {
                method: "GET",
                headers: {
                    "Authorization": `Bearer ${token}`
                }
            });

            if (!response.ok) {
                throw new Error("No s'ha pogut obtenir el servei.");
            }

            const resposta = await response.json();
            console.log("Resposta:", resposta);
            const serveis = resposta.serveis || [];

            return serveis.filter(servei => servei.ofertant_id === userId);
        } catch (error) {
            console.error(error);
            alert("Error carregant els serveis.");
            return [];
        }
    }

    function decodeToken(token) {
        try {
            const payload = token.split('.')[1];
            const decoded = atob(payload);
            return JSON.parse(decoded);
        } catch (error) {
            console.error("Error al decodificar el token", error);
            return null;
        }
    }

    function seleccionarServei(servei) {
        console.log("Servicio seleccionado:", servei);

        document.getElementById("nom").value = servei.nom;
        document.getElementById("descripcio").value = servei.descripcio;
        document.getElementById("preu").value = servei.preu;
        document.getElementById("duracio").value = servei.duracio_mins;
        document.getElementById("capacitat").value = servei.capacitat;

        const tipusServeiMapReverse = {
            1: "Esportiu",
            2: "Salut",
            3: "Benestar",
            4: "Virtual"
        };
        document.getElementById("tipus_servei").value = tipusServeiMapReverse[servei.tipus_servei_id] || "";

        if (servei.data) {
            const formattedData = new Date(servei.data);
            document.getElementById("dataHora").value = formattedData.toISOString().slice(0, 16);
        }

        document.getElementById("lloc").value = servei.ubicacio;

        form.style.display = "block";
        document.getElementById("titolFormulari").style.display = "block";

        buscarServeiDiv.style.display = "none";
        console.log("Ocultando la lista de servicios.");
        serveisEncontratsDiv.style.display = "none";

        serveiId = servei.id;
        estatId = servei.estat_id;
    }

    async function modificarServei(event) {
        event.preventDefault();

        const tipusServeiMap = {
            "Esportiu": 1,
            "Salut": 2,
            "Benestar": 3,
            "Virtual": 4
        };

        const tipusServeiId = tipusServeiMap[document.getElementById("tipus_servei").value] || null;

        const updatedServei = {
            nom: document.getElementById("nom").value,
            descripcio: document.getElementById("descripcio").value,
            preu: document.getElementById("preu").value,
            duracio_mins: document.getElementById("duracio").value,
            capacitat: document.getElementById("capacitat").value,
            tipus_servei_id: tipusServeiId,
            data: document.getElementById("dataHora").value,
            ubicacio: document.getElementById("lloc").value,
            estat_id: estatId
        };
        console.log("Datos del servicio a modificar:", updatedServei);
        try {
            const response = await fetch(`http://localhost:3000/api/serveis/modificar/${serveiId}`, {
                method: "PUT",
                headers: {
                    "Authorization": `Bearer ${token}`,
                    "Content-Type": "application/json"
                },
                body: JSON.stringify(updatedServei)
            });
            console.log("Response:", response);
            if (!response.ok) {
                throw new Error("Error al modificar el servei");
            }

            alert("Servei modificat correctament");
        } catch (error) {
            console.error(error);
            alert("No s'ha pogut modificar el servei.");
        }
    }

    form.addEventListener("submit", modificarServei);
});
