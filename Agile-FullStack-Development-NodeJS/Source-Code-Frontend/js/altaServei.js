document.addEventListener("DOMContentLoaded", () => {
    const token = localStorage.getItem("authToken");

    console.log("Token recuperado:", token);

    if (!token) {
        window.location.href = "../html/login.html";
        return;
    }

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
    const rol = decodedToken.rol;
    console.log("Rol ID:", rol);

    if (rol !== 3) {
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

    const form = document.getElementById("altaServeiForm");

    form.addEventListener("submit", async (event) => {
        event.preventDefault();

        const nom = document.getElementById("nom").value;
        const descripcio = document.getElementById("descripcio").value;
        const preu = parseFloat(document.getElementById("preu").value);
        const duracio = parseInt(document.getElementById("duracio").value, 10);
        const capacitat = parseInt(document.getElementById("capacitat").value, 10);
        const tipus_servei = document.getElementById("tipus_servei").value;
        const ubicacio = document.getElementById("ubicacio").value;
        const dataHora = document.getElementById("data_hora").value;

        console.log("Datos del formulario:", {
            nom,
            descripcio,
            preu,
            duracio,
            capacitat,
            tipus_servei,
            ubicacio,
            dataHora
        });

        const tipusServeiMap = {
            "Esportiu": 1,
            "Salut": 2,
            "Benestar": 3,
            "Virtual": 4
        };

        if (!tipusServeiMap[tipus_servei]) {
            alert("Si us plau, selecciona un tipus de servei vàlid.");
            return;
        }

        const fechaHora = new Date(dataHora);
        const fechaFormateada = `${fechaHora.getFullYear()}-${(fechaHora.getMonth() + 1).toString().padStart(2, '0')}-${fechaHora.getDate().toString().padStart(2, '0')} ${fechaHora.getHours().toString().padStart(2, '0')}:${fechaHora.getMinutes().toString().padStart(2, '0')}:${fechaHora.getSeconds().toString().padStart(2, '0')}`;

        const requestBody = {
            nom: nom,
            descripcio: descripcio,
            preu: preu,
            duracio_mins: duracio,
            capacitat: capacitat,
            ubicacio: ubicacio,
            data: fechaFormateada,
            estat_id: 1,
            tipus_servei_id: tipusServeiMap[tipus_servei]
        };

        try {
            const response = await fetch("http://localhost:3000/api/serveis/crear", {
                method: "POST",
                headers: {
                    "Authorization": `Bearer ${token}`,
                    "Content-Type": "application/json"
                },
                body: JSON.stringify(requestBody)
            });

            if (response.ok) {
                const data = await response.json();
                alert("Servei creat correctament!");
            } else {
                const errorData = await response.json();
                alert(`Error: ${errorData.message || "Error desconegut"}`);
            }
        } catch (error) {
            alert("No es va poder completar la sol·licitud.");
            console.error(error);
        }
    });
});
