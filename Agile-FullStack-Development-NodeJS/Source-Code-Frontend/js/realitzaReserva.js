// Funció per desxifrar el token JWT i obtenir les dades de l'usuari
function parseJwt(token) {
  try {
    const base64Url = token.split('.')[1]; // Part central del token (payload)
    const base64 = base64Url.replace(/-/g, '+').replace(/_/g, '/');
    const jsonPayload = decodeURIComponent(
      atob(base64)
        .split('')
        .map(c => '%' + ('00' + c.charCodeAt(0).toString(16)).slice(-2))
        .join('')
    );
    return JSON.parse(jsonPayload); // Retorna un objecte amb les dades del token
  } catch (error) {
    console.error("Token invàlid o no disponible:", error);
    return null;
  }
}

// Quan es carrega la pàgina, omplim els camps del formulari amb les dades del servei seleccionat
document.addEventListener("DOMContentLoaded", () => {
  const servei = JSON.parse(localStorage.getItem("serveiSeleccionat"));
  if (!servei) return;

  // Assignació de valors als inputs del formulari
  document.getElementById("servei-nom").value = servei.nom || "";
  document.getElementById("servei-descripcio").value = servei.descripcio || "";
  document.getElementById("servei-preu").value = `${servei.preu} crèdits`;
  document.getElementById("servei-duracio").value = `${servei.duracio_mins} minuts`;
  document.getElementById("servei-capacitat").value = `${servei.capacitat} ${servei.capacitat === 1 ? "persona" : "persones"}`;
  document.getElementById("servei-ubicacio").value = servei.ubicacio || "No disponible";
  document.getElementById("servei-data").value = servei.data
    ? new Date(servei.data).toLocaleString("ca-ES", { dateStyle: "short", timeStyle: "short" })
    : "No disponible";
});

// Quan es confirma la reserva, s'envien les dades al servidor
document.getElementById("form-confirmar-reserva").addEventListener("submit", async function (e) {
  e.preventDefault();

  const servei = JSON.parse(localStorage.getItem("serveiSeleccionat"));
  const token = localStorage.getItem("authToken");
  const userData = parseJwt(token);

  // Validació de dades mínimes
  if (!servei) {
    alert("Error: falta informació del servei.");
    return;
  }

  if (!userData) {
    alert("Error: falta informació de l'usuari.");
    return;
  }

  // Preparació de la informació a enviar al backend
  const reservaData = {
    usuari_id: userData.userId,
    servei_id: servei.id,
    data_reserva: new Date(servei.data).toISOString().slice(0, 19).replace('T', ' ')
  };
  
  try {
    // Crida a l'API per crear la reserva
    const resposta = await fetch("http://localhost:3000/api/reserves", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(reservaData),
    });

    const result = await resposta.json();

    if (resposta.ok) {
      alert(`Reserva realitzada correctament! Número de reserva: ${result.reserva.id}`);
      window.location.href = "dashboard.html";
    } else {
      alert("Error en la reserva: " + (result.error || "Desconegut"));
    }
  } catch (error) {
    console.error("Error en la petició de reserva:", error);
    alert("Error inesperat en fer la reserva.");
  }
});
