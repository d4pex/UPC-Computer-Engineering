#version 330 core

out vec4 FragColor;            // Salida de color para el fragmento
in vec4 vertex_color;          // Color interpolado desde el vertex shader

uniform bool aplicarEfecto;    // Si es true, pintamos con patrón de bandas; si false, color normal
uniform vec3 COLOR_MARRO;      // Color marrón que usamos en el patrón (RGB en [0..1])

void main() {
    if (aplicarEfecto) {
        // Obtenemos coordenadas de este fragmento (en píxeles) en la ventana
        int x = int(gl_FragCoord.x);
        int y = int(gl_FragCoord.y);

        // Determinamos si estamos en una “cinta vertical” de 10 px (dentro de cada 20 px)
        bool esCintaVertical   = (x % 20 < 10);
        // Y si estamos en una “cinta horizontal” de 10 px (dentro de cada 20 px)
        bool esCintaHorizontal = (y % 20 < 10);

        // Calculamos el índice de cinta vertical: alternamos entre 0 y 1 cada 20 px
        int cintaVerticalIndex = (x / 20) % 2;

        // Si el fragmento está dentro de una banda vertical:
        if (esCintaVertical) {
            if (cintaVerticalIndex == 0) {
                // Patrón 0: 10 px de COLOR_MARRO*0.5, luego 10 px de COLOR_MARRO, luego 10 px de COLOR_MARRO*0.5
                if (y % 40 < 20) {
                    FragColor = vec4(COLOR_MARRO * 0.5, 1.0);
                } else if (y % 40 < 30) {
                    FragColor = vec4(COLOR_MARRO, 1.0);
                } else if (y % 40 < 40) {
                    FragColor = vec4(COLOR_MARRO * 0.5, 1.0);
                }
            }
            else if (cintaVerticalIndex == 1) {
                // Patrón 1: 10 px de COLOR_MARRO, luego 30 px de COLOR_MARRO*0.5
                if (y % 40 < 10) {
                    FragColor = vec4(COLOR_MARRO, 1.0);
                } else if (y % 40 < 40) {
                    FragColor = vec4(COLOR_MARRO * 0.5, 1.0);
                }
            }
        }
        // Si no estamos en cinta vertical, pero sí en banda horizontal (de 10 px en cada 20 px)
        else if (esCintaHorizontal) {
            // Pintamos todo en color marrón (sin semitransparencia)
            FragColor = vec4(COLOR_MARRO, 1.0);
        }
        // Si no estamos en cinta vertical ni horizontal, pero x%20 está en [10,11)
        else if (x % 20 >= 10 && x % 20 < 11) {
            // Pintamos 1 px extra a la derecha en la cinta vertical, para simular borde
            FragColor = vec4(COLOR_MARRO, 1.0);
        }
        else {
            // Fuera de las bandas: descartamos este fragmento, creando área transparente
            discard;
        }
    }
    else {
        // Si no aplica el efecto, simplemente devolvemos el color interpolado del vértice
        FragColor = vertex_color;
    }
}
