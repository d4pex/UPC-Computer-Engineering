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
      </div>
    </div>
  `;

  return li;
}

function carregarDisponibilitat(dataSeleccionada) {
  const llista = document.getElementById("llista-serveis");

  if (!dataSeleccionada) {
    llista.innerHTML = "<p>Cal seleccionar una data per veure la disponibilitat.</p>";
    return;
  }

  console.log("Data seleccionada:", dataSeleccionada);

  fetch(`http://localhost:3000/api/disponibilitat?data=${dataSeleccionada}`)
    .then(res => res.json())
    .then(data => {
      console.log("Resposta del backend:", data);
      llista.innerHTML = "";

      if (!Array.isArray(data) || data.length === 0) {
        llista.innerHTML = "<p>No hi ha serveis disponibles per aquesta data.</p>";
        return;
      }

      data.forEach(servei => {
        const element = crearServeiComponent(servei);
        llista.appendChild(element);
      });
    })
    .catch(error => {
      console.error("Error carregant serveis disponibles:", error);
      llista.innerHTML = "<p>Error carregant serveis.</p>";
    });
}

document.addEventListener("DOMContentLoaded", () => {
  const inputData = document.getElementById("data-disponibilitat");

  inputData.addEventListener("change", () => {
    const dataSeleccionada = inputData.value;
    carregarDisponibilitat(dataSeleccionada);
  });
});