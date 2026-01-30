// Vertex Shader (shader.vert)

#version 330 core

// Atributos de entrada:
in vec3 vertex; // posición del vértice
in vec3 color;  // color asociado al vértice (solo para objetos que usan color)

// Uniforms de transformación:
uniform mat4 TG;    // Matriz de transformación del modelo (model)
uniform mat4 Proj;  // Matriz de proyección (projection)
uniform mat4 View;  // Matriz de vista (camera/view)

// Variable de salida al Fragment Shader:
out vec3 fcolor;   // color final interpolado que llegará al fragment shader

// Uniform para controlar si estamos pintando la puerta roja:
uniform bool porta3;

void main() {
    // Si porta3 == true, asignamos a fcolor el color rojo puro (1,0,0).
    // Esto hace que toda la geometría que se dibuje mientras "porta3"
    // esté en true aparezca roja, sin importar su atributo color.
    if (porta3 == true) {
        fcolor = vec3(1.0, 0.0, 0.0);
    } else {
        // En caso contrario, usamos el color que viene del atributo "color".
        fcolor = color;
    }
    // Calculamos la posición final del vértice:
    // gl_Position = Proj * View * TG * vec4(vertex, 1.0);
    // Orden: primero transformamos el vértice por la matriz de modelo (TG),
    // luego lo colocamos en coordenadas de cámara (View), y finalmente
    // aplicamos la proyección (Proj). El resultado se asigna a gl_Position.
    gl_Position = Proj * View * TG * vec4(vertex, 1.0);
}
