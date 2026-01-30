document.addEventListener("DOMContentLoaded", async () => {
    const token = localStorage.getItem("authToken"); // Suposant que el token s'emmagatzema aquí
    if (!token) {
        window.location.href = "../html/login.html"; // Redirigeix si no hi ha sessió activa
        return;
    }

    // Obtenir l'ID de l'usuari del token
    let userId = null;
    try {
        const payloadBase64 = token.split(".")[1]; // Obtenir la part del payload
        const payloadDecoded = JSON.parse(atob(payloadBase64)); // Decodificar Base64 a JSON
        userId = payloadDecoded.userId; // Retornar el userId
    } catch (error) {
        console.error("Error en decodificar el token:", error);
        userId = null;
    }

    const apiUrl = `http://localhost:3000/api/usuari/${userId}`; // Ajusta la URL del backend segons correspongui

    try {
        const response = await fetch(apiUrl, {
            method: "GET",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Content-Type": "application/json"
            }
        });

        if (!response.ok) {
            throw new Error("Error en obtenir dades del perfil");
        }

        const userData = await response.json();
        document.getElementById("dni").value = userData.dni;
        document.getElementById("nom").value = userData.nom;
        document.getElementById("sobrenom").value = userData.sobrenom;
        document.getElementById("mail").value = userData.mail;
        document.getElementById("telefon").value = userData.telefon;
        const aux = userData.rol_id;
        // per veure quin rol correspon amb cada id_rol
        switch (aux) {
            case 1:
                document.getElementById("rol").value = "usuari";
                break;
            case 2:
                document.getElementById("rol").value = "administrador";
                break;
            case 3:
                document.getElementById("rol").value = "profesional";
                break;
            default:
                document.getElementById("rol").value = "centre"; // Valor por defecto si aux no es 1, 2 ni 3
                break;
}

    } catch (error) {
        console.error("Error carregant el perfil:", error);
    }

    // Actualitzar les dades quan l'usuari enviï el formulari
    document.getElementById("perfilForm").addEventListener("submit", async (event) => {
        event.preventDefault();

        // Obtenir els valors del formulari
        const updatedData = {
            nom: document.getElementById("nom").value,
            sobrenom: document.getElementById("sobrenom").value,
            mail: document.getElementById("mail").value,
            telefon: document.getElementById("telefon").value
        };

        // Validar les dades del formulari
        const emailPattern = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$/; // Expressió regular per al correu electrònic
        const phonePattern = /^[0-9]{9,15}$/; // Expressió regular per al telèfon

        if (!emailPattern.test(updatedData.mail)) {
            alert("El correu electrònic no té un format vàlid.");
            return;
        }

        if (!phonePattern.test(updatedData.telefon)) {
            alert("El telèfon no té un format vàlid. Ha de tenir entre 9 i 15 dígits.");
            return;
        }

        const apiUrl = `http://localhost:3000/api/usuari/actualitzar`;
        try {
            const response = await fetch(apiUrl, {
                method: "PUT",
                headers: {
                    "Authorization": `Bearer ${token}`,
                    "Content-Type": "application/json"
                },
                body: JSON.stringify(updatedData)
            });

            if (!response.ok) {
                throw new Error("Error en actualitzar el perfil");
            }

            alert("Perfil actualitzat correctament");
        } catch (error) {
            console.error("Error actualitzant el perfil:", error);
            alert("Hi ha hagut un error en actualitzar el perfil");
        }
    });

    // Redirigir a esborrar-compte.html al fer clic a "Eliminar compte"
    document.getElementById("deleteAccount").addEventListener("click", () => {
        window.location.href = "../html/esborrarCompte.html";
    });
});