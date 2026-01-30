document.getElementById("registreForm").addEventListener("submit", async function(event) {
    event.preventDefault();
    
    const errorMessage = document.getElementById("error-message");
    errorMessage.innerHTML = "";

    const dni = document.getElementById("dni").value;
    const nom = document.getElementById("nom").value;
    const sobrenom = document.getElementById("sobrenom").value;
    const mail = document.getElementById("mail").value;
    const telefon = document.getElementById("telefon").value;
    const contrasenya = document.getElementById("password").value;
    const rol = document.getElementById("rol").value;

    // Validació del DNI
    if (!/^[0-9]{8}[A-Za-z]$/.test(dni)) {
        errorMessage.innerHTML = "Format de DNI incorrecte. Ha de ser 8 números seguits d'una lletra.";
        return;
    }

    // Validació del telèfon
    if (!/^[0-9]{9}$/.test(telefon)) {
        errorMessage.innerHTML = "El telèfon ha de tenir 9 dígits.";
        return;
    }

    // Validació de la contrasenya
    if (!/^(?=.*[0-9])(?=.*[\W_]).{8,}$/.test(contrasenya)) {
        errorMessage.innerHTML = "La contrasenya ha de tenir almenys 8 caràcters, incloure un número i un caràcter especial.";
        return;
    }

    const data = { dni, sobrenom, contrasenya, nom, mail, telefon, rol };

    try {
        const response = await fetch("http://localhost:3000/api/auth/register", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(data)
        });

        const result = await response.json();

        if (response.ok) {
            errorMessage.style.color = "green";
            errorMessage.innerHTML = "Registre exitós. Ara pots iniciar sessió.";
            errorMessage.setAttribute("style", "background-color: #d4edda; color: #155724; border: 1px solid #c3e6cb;");
            setTimeout(() => window.location.href = "../html/login.html", 2000);
        } else {
            errorMessage.style.color = "red";
            errorMessage.innerHTML = result.message || 'Error en el registre. Torna-ho a intentar.';
            errorMessage.setAttribute("style", "background-color: #f8d7da; color: #721c24; border: 1px solid #f5c6cb;");
        }
    } catch (error) {
        errorMessage.style.color = "red";
        errorMessage.innerHTML = "Error de connexió amb el servidor.";
        errorMessage.setAttribute("style", "background-color: #f8d7da; color: #721c24; border: 1px solid #f5c6cb;");
        console.error(error);
    }
});
