document.addEventListener("DOMContentLoaded", async () => {
  const llistaReserves = document.getElementById("reserves-llista");
  const btnVeureMes = document.getElementById("veure-mes");

  if (btnVeureMes) {
    btnVeureMes.addEventListener("click", () => {
      window.location.href = "../html/veureReserves.html";
    });
  }

  const token = localStorage.getItem("authToken");
    if (!token) {
      llistaReserves.innerHTML = "<li>No hi ha token d'autenticació.</li>";
      return;
    }

    function parseJwt(token) {
      try {
        const base64Url = token.split('.')[1];
        const base64 = base64Url.replace(/-/g, '+').replace(/_/g, '/');
        const jsonPayload = decodeURIComponent(atob(base64).split('').map(c =>
          '%' + ('00' + c.charCodeAt(0).toString(16)).slice(-2)
        ).join(''));
        return JSON.parse(jsonPayload);
      } catch (e) {
        return null;
      }
    }

    const payload = parseJwt(token);
    if (!payload) {
      llistaReserves.innerHTML = "<li>Error decodificant el token.</li>";
      return;
    }

  if (llistaReserves) {
    console.log("Rol:", payload.rol);
    if (payload.rol === 2) {
      llistaReserves.innerHTML = "<li>No tens permisos per veure les reserves.</li>";
      return;
    }

    try {
      const endpoint = payload.rol === 1
        ? "http://localhost:3000/api/reserves/visualitzar/jo"
        : "http://localhost:3000/api/reserves/visualitzar";

      const resReserves = await fetch(endpoint, {
        headers: {
          "Authorization": `Bearer ${token}`
        }
      });

      if (!resReserves.ok) throw new Error("Error al obtenir les reserves");

      let reserves = await resReserves.json();

      // Filtrar reservas con fecha válida
      reserves = reserves.filter(r => {
        const d = new Date(r.data_reserva);
        return !isNaN(d.getTime());
      });

      // Ordenar por fecha ascendente
      reserves.sort((a, b) => new Date(a.data_reserva) - new Date(b.data_reserva));

      // Limitar a 7
      const properes = reserves.slice(0, 7);
      llistaReserves.innerHTML = "";

      for (const reserva of properes) {
        const hora = new Date(reserva.data_reserva).toLocaleTimeString("ca-ES", {
          hour: "2-digit", minute: "2-digit"
        });

        let ubicacio = "";
        if (payload.rol === 1) {
          // Buscar la ubicación solo si es usuari
          ubicacio = await (async function obtenirUbicacioServeiPerNom(servei_nom) {
            try {
              const resServei = await fetch(`http://localhost:3000/api/serveis?nom=${encodeURIComponent(servei_nom)}`, {
                headers: {
                  "Authorization": `Bearer ${token}`
                }
              });
              if (!resServei.ok) return "Desconegut";

              const dataServei = await resServei.json();
              const servei = dataServei.serveis?.[0];
              return servei?.ubicacio || "Desconegut";
            } catch {
              return "Desconegut";
            }
          })(reserva.servei_nom);

          
        }

        const li = document.createElement("li");
        li.innerHTML = `
          <strong>${reserva.servei_nom}</strong><br>
          ${hora}${ubicacio ? ` - ${ubicacio}` : ""}
        `;
        llistaReserves.appendChild(li);
      }

      if (properes.length === 0) {
        llistaReserves.innerHTML = "<li>No tens reserves properes.</li>";
      }

    } catch (error) {
      console.error("Error carregant les reserves:", error);
      llistaReserves.innerHTML = "<li>Error al carregar les reserves.</li>";
    }
  }

  // ---------- Rànquing de crèdits ----------
  async function carregarRankingCredits(limit = 10) {
    try {
      const res = await fetch(`http://localhost:3000/api/ranking?limit=${limit}`);
      if (!res.ok) throw new Error("Error obtenint el ranking");

      const ranking = await res.json();

      // Podio (primeros 3)
      const podio = ranking.slice(0, 3);
      if (podio[0]) {
        document.getElementById("podium-1-name").textContent = podio[0].nom;
        document.getElementById("podium-1-credits").textContent = podio[0].credits + " crèdits";
      }
      if (podio[1]) {
        document.getElementById("podium-2-name").textContent = podio[1].nom;
        document.getElementById("podium-2-credits").textContent = podio[1].credits + " crèdits";
      }
      if (podio[2]) {
        document.getElementById("podium-3-name").textContent = podio[2].nom;
        document.getElementById("podium-3-credits").textContent = podio[2].credits + " crèdits";
      }

      // Lista de usuarios a partir del cuarto
      const tbody = document.querySelector("#ranking-table tbody");
      if (tbody) {
        tbody.innerHTML = "";
        ranking.slice(3).forEach((usuari, i) => {
          const tr = document.createElement("tr");
          tr.innerHTML = `
            <td>${i + 4}</td>
            <td>${usuari.nom}</td>
            <td>${usuari.credits}</td>
          `;
          tbody.appendChild(tr);
        });
      }

    } catch (err) {
      console.error("Error carregant el ranking:", err);
    }
  }

  await carregarRankingCredits();
  console.log("Rol:" + payload.rol);
  
  if (payload.rol === 1) {
    console.log("ID:", payload.userId);
  
    await carregarRankingCredits();
console.log("Rol:" + payload.rol);

if (payload.rol === 1) {
  console.log("ID:", payload.userId);

  async function carregarReptesUsuari() {
    const token = localStorage.getItem("authToken");
    const llistaReptes = document.getElementById("llista-reptes");
    if (!llistaReptes || !token) return;

    try {
      const res = await fetch(`http://localhost:3000/api/inscripcions`, {
        method: "GET",
        headers: {
          "Accept": "application/json",
          "Authorization": `Bearer ${token}`,
        }
      });

      if (!res.ok) throw new Error("Error obtenint els reptes");

      const reptes = await res.json();
      console.log("Reptes carregats:", reptes);
      llistaReptes.innerHTML = "";

      // 🔄 Filtrar no completats
      let reptesNoCompletats = reptes.filter(r => !r.completat);

      // 🗂️ Ordenar per data_fi més propera (si existeix)
      reptesNoCompletats.sort((a, b) => {
        const dataA = a.data_fi ? new Date(a.data_fi) : null;
        const dataB = b.data_fi ? new Date(b.data_fi) : null;

        if (!dataA && !dataB) return 0;       // ambdues nul·les, iguals
        if (!dataA) return 1;                 // a no té data -> va després
        if (!dataB) return -1;                // b no té data -> va després
        return dataA - dataB;                 // menor data primer
      });

      // 🧮 Limitar a 4
      reptesNoCompletats = reptesNoCompletats.slice(0, 4);

      if (reptesNoCompletats.length === 0) {
        llistaReptes.innerHTML = "<li>No tens reptes pendents.</li>";
        return;
      }

      reptesNoCompletats.forEach(r => {
        console.log("Data:", r.data_fi);
        const li = document.createElement("li");
        // Validar fecha abans de formatar
        let dataLimitStr = "Sense data";
        if (r.data_fi) {
          const dataObj = new Date(r.data_fi);
          if (!isNaN(dataObj)) {
            dataLimitStr = dataObj.toLocaleDateString();
          }
        }
        li.innerHTML = `
          <strong>${r.nom}</strong><br>
          🎯 <em>Objectiu:</em> ${r.objectiu || "No especificat"}<br>
          📅 <em>Data límit:</em> ${dataLimitStr}<br>
        `;
        llistaReptes.appendChild(li);
      });

    } catch (err) {
      console.error("Error carregant reptes:", err);
      llistaReptes.innerHTML = "<li>Error al carregar els reptes.</li>";
    }
  }

  await carregarReptesUsuari();
}


}
  


  

  if (payload.rol === 3) {
    const reptesContainer = document.getElementById("reptes-container");
    const valoracionsContainer = document.getElementById("valoracions-container");
    const missatgeBuit = document.getElementById("missatge-buit");
    const llistaValoracions = document.getElementById("llista-valoracions");
  
    // Mostrar valoracions, ocultar reptes
    reptesContainer.style.display = "none";
    valoracionsContainer.style.display = "block";
  
    try {
      const res = await fetch("http://localhost:3000/api/valoracions", {
        headers: { "Authorization": `Bearer ${token}` }
      });
  
      if (!res.ok) throw new Error("Error carregant valoracions.");
  
      const valoracions = await res.json();
      console.log("Valoracions:", valoracions);
  
      if (valoracions.length === 0) {
        missatgeBuit.style.display = "block";
      } else {
        missatgeBuit.style.display = "none";
  
        // Obtener solo las 3 últimas valoraciones
        const ultimesValoracions = valoracions.slice(-3).reverse();
  
        ultimesValoracions.forEach(val => {
          const li = document.createElement("li");
          li.classList.add("valoracio-item");
  
          const estrelles = "★".repeat(val.puntuacio) + "☆".repeat(5 - val.puntuacio);
  
          li.innerHTML = `
            <div class="valoracio-capcalera">
              <span class="usuari"><strong>Usuari:</strong> ${val.usuari_nom}</span>
              <span class="estrelles">${estrelles}</span>
            </div>
            <p><strong>Servei valorat:</strong> ${val.servei_nom}</p>
            <p><strong>Comentari:</strong> ${val.comentari}</p>
          `;
  
          llistaValoracions.appendChild(li);
        });
      }
    } catch (err) {
      console.error("Error carregant valoracions:", err);
    }
  }
  
  
  
});
