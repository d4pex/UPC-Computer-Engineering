// Diccionario de nombres de categoría
const categoriasNombres = {
    1: 'Equipament',
    2: 'Suplementació',
    3: 'Roba',
    4: 'Accessoris',
    5: 'Formació',
    6: 'Alimentació'
  };
  
  document.querySelectorAll('.boto-categoria').forEach(btn => {
    btn.addEventListener('click', () => {
      btn.classList.toggle('seleccionada');
    });
  });
  
  document.getElementById('buscar-btn').addEventListener('click', async () => {
    const categoriasSeleccionadas = Array.from(document.querySelectorAll('.boto-categoria.seleccionada'))
      .map(btn => parseInt(btn.dataset.id));
  
      try {
        const response = await fetch('http://localhost:3000/api/productes', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json'
          },
          body: JSON.stringify({ categories: categoriasSeleccionadas })
        });
      
        const productos = await response.json();
      
        // Agrupar productos por categoria
        const productosPorCategoria = {};
        productos.forEach(p => {
          if (!productosPorCategoria[p.categoria_id]) {
            productosPorCategoria[p.categoria_id] = [];
          }
          productosPorCategoria[p.categoria_id].push(p);
        });
      
        // Mostrar productos
        const contenedor = document.getElementById('resultados');
        contenedor.innerHTML = '';
      
        for (const categoriaId in productosPorCategoria) {
          const grupo = productosPorCategoria[categoriaId];
      
          const titulo = document.createElement('h3');
          titulo.textContent = categoriasNombres[categoriaId] || `Categoria ${categoriaId}`;
          contenedor.appendChild(titulo);
      
          const grid = document.createElement('div');
          grid.className = 'grid-productos';
      
          grupo.forEach(producte => {
            const card = document.createElement('div');
            card.className = 'producto';
            card.style.border = '1px solid #ccc';
            card.style.borderRadius = '8px';
            card.style.padding = '10px';
            card.style.boxShadow = '0 2px 5px rgba(0,0,0,0.1)';
            card.style.display = 'flex';
            card.style.flexDirection = 'column';
            card.style.justifyContent = 'space-between';
      
            card.innerHTML = `
              <img src="${producte.imatge}" alt="${producte.nom}"
                  style="width:100%; max-height:200px; border-radius:6px; object-fit:contain; margin-bottom:10px;">
              <strong>${producte.nom}</strong>
              <p>Preu: ${producte.preu} €</p>
              <p>Descripció: ${producte.descripcio || '—'}</p>
              <a href="${producte.url_compra}" target="_blank" rel="noopener noreferrer"
                  style="background-color: #007BFF; color: white; padding: 8px 12px;
                  text-align: center; border-radius: 5px; text-decoration: none; font-weight: 600;
                  margin-top: 10px;">Comprar</a>
            `;
            grid.appendChild(card);
          });
      
          contenedor.appendChild(grid);
        }
      
        // Mostrar resultados con margen y ocultar categorías
        document.getElementById('selector-categorias').classList.add('ocult');
        document.getElementById('resultados-container').classList.remove('ocult');

      
      } catch (error) {
        console.error("Error al cargar productes:", error);
      }
      
  });
  