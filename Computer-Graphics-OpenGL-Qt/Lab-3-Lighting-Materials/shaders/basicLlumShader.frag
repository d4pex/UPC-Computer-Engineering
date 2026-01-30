#version 330 core

// ----- Entrada desde el Vertex Shader -----
// fcolor: color difuso del material (matdiff)
// vertexPos: posición del vértice en SCA (usada para cálculos de iluminación y fog)
// Normal: normal del vértice en SCA (ya normalizada parcialmente)
// ------------------------------------------
in vec3 fcolor;
in vec3 vertexPos;
in vec3 Normal;

// ----- Uniforms generales -----
// colorAmbient: vec3 con color del componente ambiental (0.1,0.1,0.1)
// colorLlum: vec3 con color de la luz global (0.1,0.6,0.5) * intensidadFoc
// posLlum: vec3 con posición de la luz global en SCA (0,50,0)
// posCamera: vec3 con posición de la cámara en SCA (inversa de View)
// --------------------------------
uniform vec3 colorAmbient;
uniform vec3 colorLlum;
uniform vec3 posLlum;
uniform vec3 posCamera;

// ----- Uniforms de los 4 focos del submarino -----
// posFocusSub[i]: posición del foco i en SCA
// dirFocusSub[i]: dirección del cono del foco i (unitario, en SCA)
// colorFocusSub[i]: color del foco i (glm::vec3)
// conectarFocusSub[i]: bool (int 0/1) para encender/apagar foco i
// --------------------------------
uniform vec3 posFocusSub[4];
uniform vec3 dirFocusSub[4];
uniform vec3 colorFocusSub[4];
uniform bool conectarFocusSub[4];

// ----- Control de fases de dibujado -----
// faseDibuixat: false = pintamos sólidos, true = pintamos vidrios
// pintarMar:   true = estamos pintando la cara del mar (prisma); false = otro objeto
// --------------------------------
uniform bool faseDibuixat;
uniform bool pintarMar;

// ----- Matriz Proyección*Vista -----
// Se usa para proyectar coordenadas de SCA a NDC (necesario para rastro de foco en mar)
uniform mat4 projView;

// ----- Salida del Fragment Shader -----
out vec4 FragColor;

// ===========================
// Utilities: random & noise
// ===========================

// Función 'random' toma coordenadas 2D y devuelve un float pseudoaleatorio [0,1)
float random (in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Función 'noise' interpolada a partir de 4 valores aleatorios en la cuadrícula.
// Basada en la típica implementación de Perlin/simplex noise sencilla.
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    vec2 u = f*f*(3.0 - 2.0*f);
    return mix(a, b, u.x) + (c - a)*u.y*(1.0 - u.x) + (d - b)*u.x*u.y;
}

void main() {
    // ======================================================
    // 1) SI PINTAMOS EL MAR (prisma):
    //    - Generamos color dinámico con noise en las coordenadas XZ
    //    - Aplicamos mezcla con un tono claro
    //    - Añadimos efecto de focos submarinos sobre la superficie del mar
    //    - Alpha final del mar = 0.2 (transparente)
    //    - Devolvemos FragColor y finalizamos el shader
    // ======================================================
    if (pintarMar) {
        // Color base: tonos de azul
        vec3 colorBase = vec3(0.0/255.0, 50.0/255.0, 255.0/255.0);
        vec3 colorFosc = vec3(0.0/255.0, 20.0/255.0, 100.0/255.0);
        vec3 colorClar = vec3(1.0);  // blanco puro para reflejos

        // Coordenadas UV para noise: escalamos las coordenadas XZ del vértice
        vec2 uv = vertexPos.xz * 0.1;
        float n = noise(uv);  // valor ruido [0,1]

        // Mezclamos colorFosc y colorBase según n
        vec3 base = mix(colorFosc, colorBase, n);

        // Mezclamos un poco con color blanco para dar reflejos
        vec3 colorFinal = mix(base, colorClar, n * 0.2);

        // =========================================================
        // EFECTO DE LOS FOCOS SUBMARINOS SOBRE EL MAR:
        // Recorremos los 4 focos; si está apagado (conectarFocusSub[i]=false), lo saltamos.
        // Sólo afecta si el vértice del mar está por encima de y=1.0 (evitamos “luz” bajo la “arena”).
        // =========================================================
        for (int i = 0; i < 4; ++i) {
            if (!conectarFocusSub[i]) continue;
            if (vertexPos.y < 1.0) continue; // no iluminar bajo “suelo”

            // ---- PROYECCIÓN A NDC de diferentes puntos:
            // 1) foco --> clip coords --> NDC
            vec4 focus_clip = projView * vec4(posFocusSub[i], 1.0);
            vec2 focus_ndc  = focus_clip.xy / focus_clip.w;  // div. perspectiva

            // 2) un punto en la dirección del foco: desti = posFocusSub[i] + dirFocusSub[i]
            vec3 desti = posFocusSub[i] + dirFocusSub[i];
            vec4 desti_clip = projView * vec4(desti, 1.0);
            vec2 desti_ndc  = desti_clip.xy / desti_clip.w;

            // 3) el fragmento actual:
            vec4 frag_clip = projView * vec4(vertexPos, 1.0);
            vec2 frag_ndc  = frag_clip.xy / frag_clip.w;

            // ---- CÁLCULO DE CONO EN 2D:
            // dir = dirección 2D del haz: normalize(desti_ndc - focus_ndc)
            // capAFrag = dirección 2D del fragmento: normalize(frag_ndc - focus_ndc)
            vec2 dir      = normalize(desti_ndc - focus_ndc);
            vec2 capAFrag = normalize(frag_ndc - focus_ndc);

            // β = dot(dir, capAFrag) es cos(ángulo)
            float beta    = dot(dir, capAFrag);
            float cosTheta = cos(radians(8.0)); // apertura de 8º

            // Distancia relativa en NDC: distFocus = dot(relPos, dir)
            vec2 relPos = frag_ndc - focus_ndc;
            float distFocus = dot(relPos, dir);

            // Si el fragmento está dentro del cono (β > cos(θ) y está delante del foco)
            if (beta > cosTheta && distFocus > 0.0) {
                // Intensidad del haz: suavizado entre bordes del cono
                float intensitat = smoothstep(cosTheta, 1.0, beta);
                // Aumentamos color del mar con un “destello” del foco (30% máximo)
                colorFinal += intensitat * colorFocusSub[i] * 0.5;
            }
        }

        // Finalmente, devolvemos FragColor con alpha = 0.2
        FragColor = vec4(colorFinal, 0.2);
        return;
    }

    // ======================================================
    // 2) SEPARACIÓN SÓLIDOS / VIDRIOS (submarino, arrecife):
    //    - Los vértices de “cristal” (ventanas) tienen matdiff = vec3(0,0,1)
    //    - Si faseDibuixat == false (fase sólidos) y es cristal: discard
    //    - Si faseDibuixat == true (fase vidrios) y NO es cristal: discard
    // ======================================================
    bool esCristall = (fcolor == vec3(0.0, 0.0, 1.0));
    if (!faseDibuixat && esCristall) discard;
    if ( faseDibuixat && !esCristall) discard;

    // ======================================================
    // 3) ILUMINACIÓN Phong BASE (global):
    //    - componente ambiental: colorAmbient * fcolor
    //    - componente difusa: max(dot(normal, dirLlum),0) * colorLlum * fcolor
    //    - componente especular: pow(max(dot(dirVisio, dirReflex),0), 32) * colorLlum
    // ======================================================
    vec3 normal     = normalize(Normal);
    vec3 dirLlum    = normalize(posLlum - vertexPos);   // dirección de la luz global
    vec3 dirVisio   = normalize(posCamera - vertexPos); // dirección hacia la cámara
    vec3 dirReflex  = reflect(-dirLlum, normal);        // vector reflejado

    vec3 ambient    = colorAmbient * fcolor;
    float difus     = max(dot(normal, dirLlum), 0.0);
    vec3 diffuse    = difus * colorLlum * fcolor;
    float especularF= pow(max(dot(dirVisio, dirReflex), 0.0), 32.0);
    vec3 specular   = especularF * colorLlum;

    vec3 colorResultat = ambient + diffuse + specular;

    // ======================================================
    // 4) ILUMINACIÓN EXTRA DE LOS FOCOS SUBMARINOS
    //    - Para cada foco: si conectarFocusSub[i] == false, saltamos
    //    - Calculamos Phong local: dirFocus (luz->vértice), reflect, difusFS, especFS
    //    - Spotlight: calculamos cos(α) = dot(dirFocusSub[i], capAFrag)
    //      donde capAFrag = normalize(vertexPos - posFocusSub[i])
    //      Si cos(α) > cos(8º), aplicamos la parte difusa+especular multiplicada por cos(α)
    //    - Bombeta local: si distancia foco->vértice <= 0.5, ignoramos cono y sumamos Phong normal
    // ======================================================
    vec3 llumExtra = vec3(0.0);
    float cosTheta = cos(radians(8.0)); // apertura 8º

    for (int i = 0; i < 4; ++i) {
        if (!conectarFocusSub[i]) continue;

        // 4.1 Phong local (puntual) sin atenuación ni foco:
        vec3 dirFocus    = normalize(posFocusSub[i] - vertexPos);
        vec3 dirReflexFS = reflect(-dirFocus, normal);
        float difusFS    = max(dot(normal, dirFocus), 0.0);
        float especularFS= pow(max(dot(dirVisio, dirReflexFS), 0.0), 32.0);

        // 4.2 CÁLCULO SPOTLIGHT (cono):
        //    capAFrag = normalize(vertexPos - posFocusSub[i]) (vector del foco al fragmento)
        //    cosAlfa = dot(normalize(dirFocusSub[i]), capAFrag)
        vec3 capAFrag   = normalize(vertexPos - posFocusSub[i]);
        float cosAlfa   = dot(normalize(dirFocusSub[i]), capAFrag);

        if (cosAlfa > cosTheta) {
            // El fragmento está dentro del cono.
            // Por difusa, multiplicamos por fcolor * colorFocusSub[i] * difusFS * cosAlfa
            vec3 difSpot  = difusFS * colorFocusSub[i] * fcolor * cosAlfa;
            vec3 specSpot = especularFS * colorFocusSub[i] * cosAlfa;
            llumExtra += difSpot + specSpot;
        }

        // 4.3 BOMBETA LOCAL (área muy cercana <= 0.5):
        float distancia = distance(posFocusSub[i], vertexPos);
        if (distancia <= 0.5) {
            // Sumar Phong completo sin foco
            vec3 difLocal  = difusFS * colorFocusSub[i] * fcolor;
            vec3 specLocal = especularFS * colorFocusSub[i];
            llumExtra += difLocal + specLocal;
        }
    }

    // Sumamos la iluminación extra de focos al colorResultat
    colorResultat += llumExtra;

    // ======================================================
    // 5) EFECTO DE DISTANCIA (FOG / ENFOSCAMIENTO)
    //    - Calculamos la distancia entre cámara y vértice
    //    - factorBoira = smoothstep(8.0, 15.0, dist): 0 si dist<8, 1 si dist>15, interpolado en medio
    //    - colorFinal = mix(colorResultat, vec3(0.0), factorBoira) → a más distancia, más se acerca al negro
    // ======================================================
    float dist = length(posCamera - vertexPos);
    float factorBoira = smoothstep(8.0, 15.0, dist);
    vec3 colorFinal = mix(colorResultat, vec3(0.0), factorBoira);

    // ======================================================
    // 6) ALPHA SEGÚN FASE DE DIBUJADO:
    //    - Si faseDibuixat: alpha = 0.7 (ventanas translúcidas)
    //    - Si faseDibuixat == false: alpha = 1.0 (sólidos)
    // ======================================================
    float alpha = faseDibuixat ? 0.7 : 1.0;
    FragColor = vec4(colorFinal, alpha);
}
