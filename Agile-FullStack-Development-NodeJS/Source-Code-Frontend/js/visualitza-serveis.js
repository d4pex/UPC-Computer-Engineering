function crearServeiComponent(servei) {
  const li = document.createElement("li");
  li.classList.add("servei-item");

  li.innerHTML = `
    <div class="servei-detall">
      <div>
        <h2>${servei.nom}</h2>
        <p><strong>Descripció:</strong> ${servei.descripcio || "No disponible"}</p>
        <p><strong>Preu:</strong> ${servei.preu} crèdit${servei.preu > 1 ? "s" : ""}</p>
        <p><strong>Duració:</strong> ${servei.duracio_mins} minuts</p>
        <p><strong>Capacitat:</strong> ${servei.capacitat ?? "No disponible"} ${servei.capacitat === 1 ? "persona" : "persones"}</p>
        <p><strong>Ubicació:</strong> ${servei.ubicacio || "No disponible"}</p>
        <p><strong>Data:</strong> ${servei.data ? new Date(servei.data).toLocaleString("ca-ES", { dateStyle: "short", timeStyle: "short" }) : "No disponible"}</p>
        <p><strong>Estat:</strong> ${servei.estat_id === 1 ? "Disponible" : "Ple" || "Desconegut"}</p>
      </div>
      <div class="boto-container" style="text-align: right; margin-top: 1rem;"></div>
    </div>
  `;

  // Botó estilitzat
  const botoReserva = document.createElement("button");
  botoReserva.className = "btn btn-reserva";
  botoReserva.textContent = "Reservar";
  botoReserva.addEventListener("click", () => {
    localStorage.setItem("serveiSeleccionat", JSON.stringify(servei));
    window.location.href = "realitzaReserva.html";
  });

  li.querySelector(".boto-container").appendChild(botoReserva);
  return li;
}

document.addEventListener("DOMContentLoaded", () => {
  fetch("http://localhost:3000/api/serveis?limit=10")
    .then(res => res.json())
    .then(data => {
      const llista = document.getElementById("llista-serveis");
      llista.innerHTML = "";
      data.serveis.forEach(servei => {
        const element = crearServeiComponent(servei);
        llista.appendChild(element);
      });
    })
    .catch(error => {
      console.error("Error carregant serveis:", error);
    });
});