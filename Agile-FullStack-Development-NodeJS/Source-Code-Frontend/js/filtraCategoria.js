document.addEventListener("DOMContentLoaded", () => {
  const filtrarBtn = document.getElementById("filtrar-btn");
  const categoriaSelect = document.getElementById("categoria-select");
  const llistaServeis = document.getElementById("llista-serveis");

  async function carregarServeisFiltrats(categoria) {
    try {
      const url = categoria
        ? `http://localhost:3000/api/serveis/filtrats?categoria=${encodeURIComponent(categoria)}`
        : `http://localhost:3000/api/serveis/filtrats`;

      const resposta = await fetch(url);
      const serveis = await resposta.json();

      llistaServeis.innerHTML = "";

      if (serveis.length === 0) {
        llistaServeis.innerHTML = "<li>No hi ha serveis disponibles per aquesta categoria.</li>";
        return;
      }

      serveis.forEach(servei => {
        const li = document.createElement("li");
        li.classList.add("servei-item");

        li.innerHTML = `
          <div class="servei-detall">
            <div>
              <h2>${servei.nom}</h2>
              <p><strong>Descripció:</strong> ${servei.descripcio || "No disponible"}</p>
              <p><strong>Preu:</strong> ${servei.preu} crèdit${servei.preu > 1 ? "s" : ""}</p>
              <p><strong>Duració:</strong> ${servei.duracio_mins} minuts</p>
              <p><strong>Ubicació:</strong> ${servei.ubicacio || "No disponible"}</p>
              <p><strong>Data:</strong> ${servei.data ? new Date(servei.data).toLocaleString("ca-ES") : "No disponible"}</p>
              <p><strong>Estat:</strong> ${servei.estat_id === 1 ? "Disponible" : "Ple" || "Desconegut"}</p>
            </div>
            <div class="boto-container" style="text-align: right; margin-top: 1rem;"></div>
          </div>
        `;

        const botoReserva = document.createElement("button");
        botoReserva.className = "btn btn-reserva";
        botoReserva.textContent = "Reservar";
        botoReserva.addEventListener("click", () => {
          localStorage.setItem("serveiSeleccionat", JSON.stringify(servei));
          window.location.href = "realitzaReserva.html";
        });

        li.querySelector(".boto-container").appendChild(botoReserva);
        llistaServeis.appendChild(li);
      });
    } catch (error) {
      console.error("Error carregant serveis filtrats:", error);
      llistaServeis.innerHTML = "<li>Error en carregar els serveis.</li>";
    }
  }

  filtrarBtn.addEventListener("click", () => {
    const categoriaSeleccionada = categoriaSelect.value;
    carregarServeisFiltrats(categoriaSeleccionada);
  });

  carregarServeisFiltrats(null);
});