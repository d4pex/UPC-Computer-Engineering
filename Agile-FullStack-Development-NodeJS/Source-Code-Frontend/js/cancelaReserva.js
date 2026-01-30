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

document.getElementById("cancel-form").addEventListener("submit", async (e) => {
  e.preventDefault();

  const params = new URLSearchParams(window.location.search);
  const reservaId = params.get("reservaId");
  const token = localStorage.getItem("authToken");
  const userData = parseJwt(token);

  if (!token || !userData) {
    alert("Usuari no autenticat.");
    return;
  }

  console.log("Usuari ID extret del token:", userData.userId); // ✅ Mostra l'ID a la consola
  console.log("ID de la reserva rebuda:", reservaId); // 🔍 Mostra per consola


  try {
    const resposta = await fetch(`http://localhost:3000/api/reserves/${reservaId}/cancelar`, {
      method: "PUT",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify({ usuari_id: userData.userId })
    });

    const resultat = await resposta.json();

    if (resposta.ok) {
      alert("Reserva cancel·lada correctament.");
      window.location.href = "dashboard.html";
    } else {
      alert("Error: " + resultat.error);
    }
  } catch (err) {
    console.error("Error cancel·lant reserva:", err);
    alert("Error inesperat.");
  }
});