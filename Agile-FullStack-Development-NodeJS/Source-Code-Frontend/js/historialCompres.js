document.addEventListener("DOMContentLoaded", () => {
    const lista = document.getElementById("historial-list");
    const notification = document.getElementById("notification-historial");
    const token = localStorage.getItem("authToken");
    if (!lista || !notification) return;
    if (!token) {
        window.location.href = "../html/login.html";
        return;
    }

    async function cargarHistorial() {
        try {
            const res = await fetch("http://localhost:3000/api/usuari/transaccions", {
                headers: { "Authorization": `Bearer ${token}` }
            });
            const data = await res.json();
            if (!res.ok) throw new Error(data.error || "Error carregant historial.");

            const trans = data.transactions.filter(t => t.tipus === "compra");
            if (trans.length === 0) {
                showNotification("No tens compres realitzades.", "success");
                return;
            }

            // Neteja la llista i la notificació
            lista.innerHTML = "";
            notification.classList.remove("show");

            // Ordenar las transacciones de más reciente a más antigua
            trans.sort((a, b) => new Date(b.data_transaccio) - new Date(a.data_transaccio));

            // Crear una targeta per a cada transacció
            trans.forEach(t => {
                const card = document.createElement("div");
                card.className = "pack";  // Clase para las tarjetas

                const d = new Date(t.data_transaccio);
                card.innerHTML = `
                    <img src="../img/credits-logo.png" alt="Paquet de crèdits">
                    <h3>Compra de ${t.quantitat} crèdits</h3>
                    <p><strong>Data:</strong> ${d.toLocaleDateString()} ${d.toLocaleTimeString()}</p>
                    <p><strong>Import:</strong> ${t.preu !== null ? t.preu + " €" : "—"}</p>
                    <p>${t.descripcio}</p>
                `;

                lista.append(card);
            });
        } catch (err) {
            console.error(err);
            showNotification(err.message || "Error en carregar historial.", "error");
        }
    }

    function showNotification(message, type) {
        notification.textContent = message;
        notification.classList.remove("notification-error", "notification-success", "hide");
        notification.classList.add(
            type === "success" ? "notification-success" : "notification-error",
            "show"
        );
        setTimeout(() => {
            notification.classList.remove("show");
            notification.classList.add("hide");
        }, 3000);
    }

    cargarHistorial();
});
