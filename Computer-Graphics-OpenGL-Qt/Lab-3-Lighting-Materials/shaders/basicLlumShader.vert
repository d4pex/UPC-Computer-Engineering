#version 330 core

// ----- Atributos de entrada -----
in vec3 vertex;   // posición del vértice en SCM (viene del VBO)
in vec3 normal;   // normal del vértice en SCM (viene del VBO)

// Atributos de material (pasados como atributos para simplicidad, pero podrían ser uniforms):
// matamb: componente ambiental del material
// matdiff: componente difusa del material (color principal)
// matspec: componente especular del material (para Phong)
// matshin: shininess (exponente especular)
// --------------------------------
in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

// ----- Salidas al fragment shader -----
out vec3 fcolor;    // color difuso que usará el fragment shader
out vec3 vertexPos; // posición en SCA (mundo) del vértice (para iluminación)
out vec3 Normal;    // normal transformada a SCA (mundo) (para iluminación)

// ----- Uniforms de transformación -----
uniform mat4 proj;   // matriz Proyección (se envía desde C++ con projectTransform())
uniform mat4 view;   // matriz Vista (se envía desde C++ con viewTransform())
uniform mat4 TG;     // matriz Transformación del modelo (submarino, reef, mar…)
// --------------------------------------

void main()
{
    // 1. Calculamos la posición en SCA: pos = TG * vec4(vertex,1.0)
    vec4 pos = TG * vec4(vertex, 1.0);

    // 2. Pasamos vertexPos al fragment shader (descarte de la componente w):
    vertexPos = vec3(pos);

    // 3. Transformación de la normal a SCA:
    //    - La normal en SCM es 'normal'. Para llevarla a SCA, usamos la matriz inversa-transpuesta de TG:
    //      normal_mundo = mat3(transpose(inverse(TG))) * normal
    //    - Esto compensa escalados/rotaciones. No necesitamos el componente de traslación.
    Normal = mat3(transpose(inverse(TG))) * normal;

    // 4. Pasamos al fragment shader fcolor = matdiff (color difuso del material)
    fcolor = matdiff;

    // 5. Calculamos la posición final del vértice en clip-space:
    //    gl_Position = proj * view * pos
    gl_Position = proj * view * pos;
}
