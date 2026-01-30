document.addEventListener("DOMContentLoaded", () => {
    const form = document.getElementById("payment-form");
    const notification = document.getElementById("notification");
    if (!form || !notification) return;
  
    form.addEventListener("submit", async (e) => {
      e.preventDefault();
  
      // 1) Token de autenticació
      const token = localStorage.getItem("authToken");
      if (!token) {
        window.location.href = "../html/login.html";
        return;
      }
  
      // 2) Llegir valors del formulari
      const titular    = document.getElementById("nomTitular").value.trim();
      const compte     = document.getElementById("compteBancari").value.trim();
      const pin        = document.getElementById("pin").value.trim();
      const dataCadStr = document.getElementById("dataCaducitat").value; // format "YYYY-MM-DD"
      const credits    = parseInt(document.getElementById("credits-a-comprar").value, 10);
  
      const errors = [];
  
      // 3) Validacions
      if (!/^[a-zA-ZÀ-ÿ\s]+$/.test(titular)) {
        errors.push("El nom del titular no és vàlid.");
      }
      if (!/^\d{4}-\d{4}-\d{4}-\d{4}$/.test(compte)) {
        errors.push("El compte bancari ha de seguir el format 4-4-4-4.");
      }
      if (!/^\d{3}$/.test(pin)) {
        errors.push("El PIN ha de contenir exactament 3 dígits.");
      }
  
      // Data de caducitat futura
      const dataCad = new Date(dataCadStr);
      const avui    = new Date(); avui.setHours(0,0,0,0);
      if (!dataCadStr || isNaN(dataCad) || dataCad <= avui) {
        errors.push("La data de caducitat ha de ser posterior a avui.");
      }

      const preus = {
        20: 2.5,
        50: 5,
        100: 9,
        150: 12,
        250: 18,
        400: 28,
        600: 40,
        1000: 60
      };

      const preu = preus[credits];
      if (!preu) {
        errors.push("Quantitat de crèdits no vàlida.");
      }
  
      if (errors.length) {
        showNotification(errors.join("\n"), "error");
        return;
      }
  
      try {
        // 4) Enviar pagament
        const pagoRes = await fetch("http://localhost:3000/api/usuari/pagament", {
          method: "POST",
          headers: {
            "Authorization": `Bearer ${token}`,
            "Content-Type": "application/json"
          },
          body: JSON.stringify({
            quantitat: credits,
            preu: preu,
            metode_pagament: "targeta",
            descripcio: `Compra de ${credits} crèdits amb targeta`
          })
        });
        const pagoData = await pagoRes.json();
        if (!pagoRes.ok || !pagoData.success) {
          throw new Error(pagoData.error || pagoData.message || 'Pagament rebutjat.');
        }
  
        // 6) Missatge d'èxit i redirigir
        showNotification(`Pagament processat correctament! Has afegit ${credits} crèdits.`, "success");
        window.scrollTo(0, 0);
        setTimeout(() => window.location.href = "compraCredits.html", 2000);
  
      } catch (err) {
        console.error(err);
        showNotification(err.message || "Error en el pagament.", "error");
        window.scrollTo(0, 0);
      }
    });
  });
  
  // Mostrar notificació a l'usuari
  function showNotification(message, type) {
    const notif = document.getElementById("notification");
    notif.textContent = message;
    notif.classList.remove("notification-error", "notification-success", "hide");
    notif.classList.add(type === "success" ? "notification-success" : "notification-error", "show");
    // Després d'uns segons, amagar
    setTimeout(() => {
      notif.classList.remove("show");
      notif.classList.add("hide");
    }, 3000);
  }