document.querySelectorAll(".reservar-btn").forEach((button) => {
  button.addEventListener("click", async function () {
    const plan = this.closest(".plan");
    const planName = plan.querySelector("h2").textContent;

    const planIds = {
      Prova: 1,
      Essencial: 2,
      Avançat: 3,
      Premium: 4,
    };

    let userId;

    const token = localStorage.getItem("authToken");
    console.log("Token recuperado:", token);

    if (token) {
      try {
        const payload = JSON.parse(atob(token.split(".")[1]));
        console.log("Payload decodificado:", payload); 
        userId = payload.userId; 
      } catch (error) {
        console.error("Error al decodificar el token:", error);
      }
    }

    const subscripcio_id = planIds[planName];
    console.log("subscripcio_id:", subscripcio_id);

    const data = {
      usuari_id: userId,
      subscripcio_id: subscripcio_id,
    };

    try {
      const response = await fetch("http://localhost:3000/api/subscripcions/seleccionar", {
        method: "PUT",
        headers: {
          "Content-Type": "application/json",
          Authorization: `Bearer ${token}`,
        },
        body: JSON.stringify(data),
      });

      const result = await response.json();

      if (response.ok) {
        alert("Pla reservat exitosament.");
      } else if (response.status === 400) {
        alert("Error: Aquest ja és el teu pla actual");
      } else {
        alert("Error al reservar el plan: " + (result.message || "Inténtalo de nuevo."));
      }
    } catch (error) {
      alert("Hubo un error al conectar con el servidor.");
      console.error(error);
    }
  });
});
