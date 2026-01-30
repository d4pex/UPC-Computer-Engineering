document.addEventListener("DOMContentLoaded", async () => {
    const llista = document.getElementById("llista-valoracions");
    const filtreInput = document.getElementById("filtre-servei");
    const botoFiltrar = document.getElementById("boto-filtrar");
    const estrellesFiltre = document.querySelectorAll(".estrella-filtre");
    const botoReset = document.getElementById("boto-reset");
    const missatgeBuit = document.getElementById("missatge-buit");

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
    console.log("Rol:", rol);
    // Verificar si el rol_id es diferente a 3 y redirigir si es necesario
    if (rol === 1) {
        window.location.href = "../html/crearValoracions.html";
        return;
    }else if (rol !== 3) {
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
    

    let valoracions = [];
    let puntuacioFiltrada = 0;

    const actualitzaEstatReset = () => {
        const textActiu = filtreInput.value.trim() !== "";
        const estrellaActiva = puntuacioFiltrada !== 0;
        botoReset.disabled = !(textActiu || estrellaActiva);
    };

    const renderValoracions = (filtres = "") => {
        llista.innerHTML = "";

        const filtrades = valoracions.filter(val => {
            const coincideixText = val.servei_nom.toLowerCase().includes(filtres.toLowerCase());
            const coincideixEstrelles = puntuacioFiltrada === 0 || val.puntuacio === puntuacioFiltrada;
            return coincideixText && coincideixEstrelles;
        });

        if (filtrades.length === 0) {
            missatgeBuit.style.display = "block";
        } else {
            missatgeBuit.style.display = "none";
        }

        filtrades.forEach(val => {
            const li = document.createElement("li");
            li.classList.add("valoracio-item");

            const estrelles = "★".repeat(val.puntuacio) + "☆".repeat(5 - val.puntuacio);

            li.innerHTML = `
                <div class="valoracio-capcalera">
                    <span class="valoracio-usuari">${val.usuari_nom}</span>
                    <span class="estrelles">${estrelles}</span>
                </div>
                <p><strong>Servei valorat:</strong> ${val.servei_nom}</p>
                <p><strong>Comentari:</strong> ${val.comentari}</p>
            `;

            llista.appendChild(li);
        });

        actualitzaEstatReset();
    };

    try {
        const res = await fetch("http://localhost:3000/api/valoracions", {
            headers: { "Authorization": `Bearer ${token}` }
        });

        if (!res.ok) throw new Error("No s'han pogut carregar les valoracions.");

        valoracions = await res.json();
        renderValoracions();

        // Filtrar en temps real mentre s'escriu
        filtreInput.addEventListener("input", () => {
            renderValoracions(filtreInput.value);
        });

        // Filtres per estrelles
        estrellesFiltre.forEach(estrella => {
            estrella.addEventListener("click", () => {
                const valor = parseInt(estrella.dataset.valor);
                puntuacioFiltrada = puntuacioFiltrada === valor ? 0 : valor;

                estrellesFiltre.forEach(e => {
                    const v = parseInt(e.dataset.valor);
                    if (v <= puntuacioFiltrada) {
                        e.classList.add("seleccionada");
                    } else {
                        e.classList.remove("seleccionada");
                    }
                });

                renderValoracions(filtreInput.value);
            });
        });

        // Reiniciar filtres
        botoReset.addEventListener("click", () => {
            filtreInput.value = "";
            puntuacioFiltrada = 0;
            estrellesFiltre.forEach(e => e.classList.remove("seleccionada"));
            renderValoracions();
        });

    } catch (err) {
        console.error("Error carregant les valoracions:", err);
    }
});
