document.addEventListener("DOMContentLoaded", () => {
  const btnFiltrar = document.getElementById("filtrar-btn");
  const categoriaSelect = document.getElementById("categoria-select");
  const dataInput = document.getElementById("data-disponibilitat");
  const llista = document.getElementById("llista-serveis");

  btnFiltrar.addEventListener("click", async () => {
    const categoria = categoriaSelect.value;
    const data = dataInput.value;

    if (!categoria && !data) {
      alert("Si us plau, selecciona almenys una categoria o una data.");
      return;
    }

    try {
      let serveisCategoria = [];
      let serveisDisponibles = [];

      if (categoria) {
        const resposta = await fetch(`http://localhost:3000/api/serveis/filtrats?categoria=${categoria}`);
        serveisCategoria = await resposta.json();
      }

      if (data) {
        const resposta = await fetch(`http://localhost:3000/api/disponibilitat?data=${data}`);
        serveisDisponibles = await resposta.json();
      }

      let serveisFinals = [];

      if (categoria && data) {
        const idsDisponibles = serveisDisponibles
          .filter(s => s.estat === "Disponible")
          .map(s => s.id || s.servei_id);
        serveisFinals = serveisCategoria.filter(s => idsDisponibles.includes(s.id));
      } else if (categoria) {
        serveisFinals = serveisCategoria;
      } else if (data) {
        serveisFinals = serveisDisponibles.filter(s => s.estat === "Disponible");
      }

      // Renderitzar serveis
      llista.innerHTML = "";
      if (serveisFinals.length === 0) {
        llista.innerHTML = "<p>No hi ha serveis disponibles segons el filtre seleccionat.</p>";
        return;
      }

      serveisFinals.forEach(servei => {
        const li = document.createElement("li");
        li.classList.add("servei-item");

        const btnReservar = document.createElement("button");
        btnReservar.textContent = "Reservar";
        btnReservar.classList.add("btn", "btn-reserva");
        btnReservar.addEventListener("click", () => {
          localStorage.setItem("serveiSeleccionat", JSON.stringify(servei));
          window.location.href = "realitzaReserva.html";
        });

        li.innerHTML = `
          <div class="servei-detall">
            <h2>${servei.nom}</h2>
            <p><strong>Descripció:</strong> ${servei.descripcio || "No disponible"}</p>
            <p><strong>Preu:</strong> ${servei.preu} crèdit${servei.preu > 1 ? "s" : ""}</p>
            <p><strong>Duració:</strong> ${servei.duracio_mins} minuts</p>
            <p><strong>Capacitat:</strong> ${servei.capacitat}</p>
            <p><strong>Ubicació:</strong> ${servei.ubicacio || "No disponible"}</p>
            <p><strong>Data:</strong> ${servei.data ? new Date(servei.data).toLocaleString("ca-ES", { dateStyle: "short", timeStyle: "short" }) : "No disponible"}</p>
            ${servei.estat ? `<p><strong>Estat:</strong> ${servei.estat}</p>` : ""}
            <p><strong>Estat:</strong> ${servei.estat_id === 1 ? "Disponible" : "Ple" || "Desconegut"}</p>
          </div>
        `;

        li.querySelector(".servei-detall").appendChild(btnReservar);
        llista.appendChild(li);
      });

    } catch (error) {
      console.error("Error aplicant el filtre:", error);
      llista.innerHTML = "<p>Error carregant els serveis filtrats.</p>";
    }
  });
});
