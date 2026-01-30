#include "MyGLWidget.h"
#include <QColorDialog>  // Para abrir el diálogo de selección de color
#include <iostream>      // Útil para posibles mensajes de debug (aunque no se usa demasiado aquí)

// Constructor: simplemente llama al constructor base de A3GLWidget
MyGLWidget::MyGLWidget (QWidget* parent) : A3GLWidget(parent) {}

// Destructor: actualmente no necesita liberar nada específico, así que vacío
MyGLWidget::~MyGLWidget() {}


// -------------------------
// initializeGL()
// -------------------------
// Se llama una vez cuando el contexto OpenGL ya está creado.
// -------------------------
void MyGLWidget::initializeGL()
{
    // Llamamos a la implementación de la clase base, que:
    //  - Carga y compila los shaders
    //  - Inicializa A3GLWidget::View, A3GLWidget::Proj, etc.
    A3GLWidget::initializeGL();

    // Activar blending (mezcla de colores) para transparencias.
    // Lo necesitaremos más adelante para el submarino translúcido y el mar translúcido.
    glEnable(GL_BLEND);
}


// -------------------------
// iniEscena()
// -------------------------
// Se llama para inicializar la escena: cargar buffers de modelos/vbo/vao, fijar centro/radio, etc.
// -------------------------
void MyGLWidget::iniEscena()
{
    // 1. Creamos los VAOs/VBOs de los modelos cargados en Model (submarino, arrecife, mar…)
    //    Estos métodos vienen de A3GLWidget e internamente llaman a creaBuffersXXXXXX()
    creaBuffersModels();

    // 2. Creamos un VAO + VBO de un cubo “debug” (al que llamamos VAO_Cub_Debug),
    //    con color blanco (1,1,1) y lado = 0.5f. Esto nos sirve para dibujar pequeñas cajas en
    //    las posiciones de los focos del submarino, con fines de depuración.
    creaBuffersQuadrat(glm::vec3(1.0,1.0,1.0), &VAO_Cub_Debug, 0.5f);

    // 3. Definimos el centro de la escena y el radio de la esfera envolvente (para cámara).
    //    El enunciado recomendaba algo que permita ver todo, aquí se toma:
    centreEsc = glm::vec3 (0, 2, 0);  // punto medio aproximado (X=0, Y=2, Z=0)
    radiEsc   = 10;                 // radio aproximado de todo el conjunto

    // 4. Posición inicial del submarino en coordenadas de la escena (SCA)
    //    (punto que se actualizará con las teclas W/A/S/D/Up/Down).
    SubmarinePos = glm::vec3(1.0, 4.0, 2.0);

    // 5. Posiciones de los 4 focos en el sistema de coordenadas del modelo (SCM) del submarino.
    //    Estas coordenadas salen directamente del enunciado:
    posFocusSubModel = {
        glm::vec3( 0.44f, 0.85f, 3.47f ),
        glm::vec3( 0.88f, 0.85f, 2.83f ),
        glm::vec3(-0.95f, 0.85f, 2.83f ),
        glm::vec3(-0.45f, 0.85f, 3.47f )
    };
}


// -------------------------
// carregaUniformLocations()
// -------------------------
// Se llama una vez después de compilar y vincular el programa de shaders. Obtiene las ubicaciones
// de los uniforms definidos en el Vertex/Fragment Shader, para poder actualizar su valor más tarde.
// -------------------------
void MyGLWidget::carregaUniformLocations()
{
    // 1. Uniforms de iluminación general (Phong global)
    colAmbLoc    = glGetUniformLocation(program->programId(), "colorAmbient");
    colLlumLoc   = glGetUniformLocation(program->programId(), "colorLlum");
    posLlumLoc   = glGetUniformLocation(program->programId(), "posLlum");
    posCameraLoc = glGetUniformLocation(program->programId(), "posCamera");

    // 2. Uniforms para controlar fases de dibujado y mar
    faseDibuixatLoc = glGetUniformLocation(program->programId(), "faseDibuixat");
    pintarMarLoc    = glGetUniformLocation(program->programId(), "pintarMar");

    // 3. Uniforms de los 4 focos del submarino (arrays en el fragment shader):
    //    Tienen nombres como "colorFocusSub[0]", "conectarFocusSub[0]", "posFocusSub[0]", "dirFocusSub[0]", etc.
    for (int i = 0; i < 4; ++i) {
        // Para cada foco i, obtenemos:
        //   - colorFocusSub[i]   (vec3)
        //   - conectarFocusSub[i] (bool)
        //   - posFocusSub[i]      (vec3)
        //   - dirFocusSub[i]      (vec3)
        colFocusSubLoc[i]   = glGetUniformLocation(program->programId(),
                                                 ("colorFocusSub[" + std::to_string(i) + "]").c_str());
        conectarFocusSub[i] = glGetUniformLocation(program->programId(),
                                                   ("conectarFocusSub[" + std::to_string(i) + "]").c_str());
        posFocusSubLoc[i]   = glGetUniformLocation(program->programId(),
                                                 ("posFocusSub[" + std::to_string(i) + "]").c_str());
        dirFocusSubLoc[i]   = glGetUniformLocation(program->programId(),
                                                 ("dirFocusSub[" + std::to_string(i) + "]").c_str());
    }

    // 4. Uniform para la matriz combinada Proyección * Vista
    projViewLoc = glGetUniformLocation(program->programId(), "projView");
}


// -------------------------
// paintGL()
// -------------------------
// Se llama cada vez que Qt necesita volver a dibujar la escena. Aquí definimos toda la lógica
// de renderizado de la Actividad 3, punto por punto.
// -------------------------
void MyGLWidget::paintGL()
{
    // 1. Fondo del mar: limpiamos el buffer de color y profundidad con un color azul oscuro.
    glClearColor(0.0f/255.0f, 44.0f/255.0f, 61.0f/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // -------------------------
    // 2. Dibujamos el MAR (prisma) como un objeto translúcido
    //    - Enviamos pintarMar = 1 para que el fragment shader aplique ruido y coloridad especial
    //    - Desactivamos máscara de profundidad para que se vea el blending correctamente
    //    - Usamos glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) para mezclar con lo que hay debajo
    //    - Luego restauramos la máscara de profundidad y el modo de blending por defecto
    // -------------------------
    glUniform1i(pintarMarLoc, 1);    // le decimos al fragment shader “estoy pintando el mar”
    glDepthMask(GL_FALSE);           // no escribir en el depth buffer (solo lectura)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO_models[SEA]);    // VAO del modelo del mar
    modelTransformSea();                   // Aplica la transformación del mar
    // Dibujamos todos los triángulos que componen el prisma (número de caras * 3 vértices)
    glDrawArrays(GL_TRIANGLES, 0, models[SEA].faces().size() * 3);
    // Restauramos flags: ahora dejaremos de pintar mar
    glUniform1i(pintarMarLoc, 0);
    glDepthMask(GL_TRUE);             // volvemos a escribir en depth buffer
    glBlendFunc(GL_ONE, GL_ZERO);     // blending desactivado (pinta como normal: fragmento dest = fragmento fuente)

    // -------------------------
    // 3. Configuramos las matrices de Proyección y Vista (cámara)
    //    Estos métodos vienen de A3GLWidget
    // -------------------------
    projectTransform();  // calcula la matriz de proyección (Proj) y la envía al vertex shader
    viewTransform();     // calcula la vista (View) y la envía al vertex shader

    // -------------------------
    // 4. Iluminación GLOBAL (Phon g) – luz puntual blavosa en (0,50,0)
    //    - Enviamos colorAmbient = (0.1,0.1,0.1)
    //    - colorLlum = (0.1,0.6,0.5) * intensitatFoc
    //    - posLlum = (0,50,0)
    //    - posCamera = posición de la cámara (extraída de View inversa)
    // -------------------------
    // 4.1 Ambiente fija (0.1,0.1,0.1)
    glUniform3f(colAmbLoc, 0.1f, 0.1f, 0.1f);

    // 4.2 Color de la luz global, multiplicamos por el factor de intensidad (0..1)
    glm::vec3 colLlum = glm::vec3(0.1f, 0.6f, 0.5f) * intensitatFoc;
    glUniform3fv(colLlumLoc, 1, &colLlum[0]);

    // 4.3 Posición de la luz global en el mundo
    glUniform3f(posLlumLoc, 0.0f, 50.0f, 0.0f);

    // 4.4 Obtener posición de la cámara (necesaria para el componente especular de Phong)
    //     View es la matriz 4x4 de la vista. Invertimos View y extraemos la 4ª columna (posición).
    glm::vec3 camPos = glm::vec3(glm::inverse(View)[3]);
    glUniform3fv(posCameraLoc, 1, &camPos[0]);

    // -------------------------
    // 5. Cálculo de transformación del submarino (TG) para determinar dónde están los focos en SCA
    //    - Partimos de identidad, lo trasladamos a SubmarinePos en SCA
    //    - Luego escalamos según la escala asignada a SUBMARINE en A3GLWidget
    //    - Luego rotamos -90º alrededor de Y (para que “mire” en la dirección correcta)
    // -------------------------
    glm::mat4 TG = glm::translate(glm::mat4(1.0f), SubmarinePos);
    TG = glm::scale(TG, glm::vec3(escalaModels[SUBMARINE]));       // escala del modelo submarino
    TG = glm::rotate(TG, glm::radians(-90.0f), glm::vec3(0,1,0));   // rotamos para orientar el modelo

    // -------------------------
    // 6. Para cada uno de los 4 focos del submarino:
    //    - Enviamos colorFocusSub[i], conectarFocusSub[i]
    //    - Calculamos posición en SCA: TG * (posición en SCM[i],1)
    //    - Calculamos dirección del cono:
    //         antena = (0.0, 2.44, 1.37) en SCM
    //         dir en SCM = posFocusSubModel[i] - antena
    //         dir en SCA = normalize(mat3(TG) * dir en SCM)
    //      Es decir, la dirección del foco apunta “desde la antena hacia la bombilla del foco”
    //    - Enviamos posFocusSubLoc[i] y dirFocusSubLoc[i]
    // -------------------------
    for (int i = 0; i < 4; ++i) {
        // 6.1 Color (glm::vec3) y encendido/apagado (int 1 o 0)
        glUniform3fv(colFocusSubLoc[i], 1, &colFocusSubVec[i][0]);
        glUniform1i(conectarFocusSub[i], focusConectat[i] ? 1 : 0);

        // 6.2 Posición del foco en SCA: TG * (posición en SCM, 1.0)
        glm::vec3 posSCA = glm::vec3(TG * glm::vec4(posFocusSubModel[i], 1.0f));
        glUniform3fv(posFocusSubLoc[i], 1, &posSCA[0]);

        // 6.3 Dirección del foco:
        //     - suponemos que la “antena” del submarino (origen del haz) está en SCM en (0,2.44,1.37)
        //     - construimos vector en SCM: posFocusSubModel[i] - antena (apunta desde antena hacia bombilla)
        //     - lo transformamos a SCA usando la parte lineal de TG: mat3(TG)*(vector)
        //     - lo normalizamos.
        glm::vec3 antena(0.0f, 2.44f, 1.37f);
        glm::vec3 dirSCA = glm::normalize(glm::mat3(TG) * (posFocusSubModel[i] - antena));
        glUniform3fv(dirFocusSubLoc[i], 1, &dirSCA[0]);
    }

    // -------------------------
    // 7. Dibujamos primero la fase 0: objetos sólidos (arrecife + submarino sólido)
    //    - Fijamos faseDibuixat = 0
    //    - Desactivamos blending con glBlendFunc(GL_ONE, GL_ZERO) (pintado normal)
    // -------------------------
    glUniform1i(faseDibuixatLoc, 0);
    glBlendFunc(GL_ONE, GL_ZERO);  // modo de blending que equivale a “sin blending” (fuente reemplaza destino)

    // 7.1 Dibujamos el arrecife (REEF)
    glBindVertexArray(VAO_models[REEF]);
    modelTransformReef();  // aplicamos la transformada del arrecife (posición/escala/rotación)
    glDrawArrays(GL_TRIANGLES, 0, models[REEF].faces().size() * 3);

    // 7.2 Dibujamos el submarino SÓLIDO (fase = 0, por eso el shader descarta polígonos cristalinos)
    glBindVertexArray(VAO_models[SUBMARINE]);
    modelTransformSubmarine();  // aplicamos transformación del submarino
    glDrawArrays(GL_TRIANGLES, 0, models[SUBMARINE].faces().size() * 3);

    // -------------------------
    // 8. FASE 1: dibujamos las partes translúcidas del submarino (los “vidrios”)
    //    - Desactivamos máscara de profundidad (para ver las ventanas a través de los sólidos)
    //    - Activamos blending alpha con glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    //    - Fijamos faseDibuixat = 1; el shader sabe que debe pintar solo vértices con matdiff == (0,0,1)
    //    - Luego volvemos a activar la máscara de profundidad y recuperaríamos faseDibuixat = 0
    // -------------------------
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniform1i(faseDibuixatLoc, 1);
    // Reutilizamos el mismo VAO y transformación del submarino, pues ya está enlazado:
    glDrawArrays(GL_TRIANGLES, 0, models[SUBMARINE].faces().size() * 3);
    glDepthMask(GL_TRUE);
    glUniform1i(faseDibuixatLoc, 0);

    // -------------------------
    // 9. DIBUJAR CUBO “DEBUG” en cada posición de foco (solo para verificar que estén bien colocados)
    //    - Usamos el VAO_Cub_Debug (creado en iniEscena)
    //    - Para cada posFocusSubModel[i], calculamos pw = TG * (pf,1) => posición en SCA
    //    - Llamamos a modelTransformDebug(pw) para situar el cubo en pw y dibujar 36 vértices
    // -------------------------
    glBindVertexArray(VAO_Cub_Debug);
    for (auto &pf : posFocusSubModel) {
        // pf es la posición en SCM. La llevamos a SCA multiplicando por TG:
        glm::vec3 pw = glm::vec3(TG * glm::vec4(pf, 1.0f));
        modelTransformDebug(pw);  // aplica transformación “mover cubo a pw y escalarlo”
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // -------------------------
    // 10. Actualizamos la matriz combinada Proyección*Vista (projView)
    //     - En el fragment shader la usaremos para convertir posiciones del foco a NDC,
    //       a fin de dibujar el rastro del foco sobre las caras internas del prisma-mar.
    //
    //     Se vuelve a calcular la proyección local aquí porque puede haber cambiado
    //     la ventana (aspecto) o la cámara. Obtenemos Proj_local y
    //     luego projView = Proj_local * View.
    // -------------------------
    float fov = float(M_PI / 3.0f);               // FOV inicial (60º) = π/3 rad
    float zn  = radiEsc * 0.25f;                  // near plane = radioEsc/4
    float zf  = 3.0f * radiEsc;                   // far plane = 3·radioEsc
    float rav = float(width()) / float(height()); // aspect ratio actual
    if (rav < ra)  // ra es aspecto original guardado en A3GLWidget, para evitar deformación
        fov = 2.0f * atan(tan(0.5f * fov) / rav);

    glm::mat4 Proj_local = glm::perspective(fov, rav, zn, zf);
    glm::mat4 projView   = Proj_local * View;
    glUniformMatrix4fv(projViewLoc, 1, GL_FALSE, &projView[0][0]);

    // -------------------------
    // 11. DIBUJAR MAR DE NUEVO SOBRE VENTANAS
    //     - Para que el efecto de mar translúcido se vea sobre las ventanas (fase 1).
    //     - Marcamos pintarMar = 1 otra vez, desactivamos depth write, blend alpha,
    //       y dibujamos el VAO del mar con modelTransformSea(). Luego restauramos flags.
    // -------------------------
    glUniform1i(pintarMarLoc, 1);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO_models[SEA]);
    modelTransformSea();
    glDrawArrays(GL_TRIANGLES, 0, models[SEA].faces().size() * 3);
    glUniform1i(pintarMarLoc, 0);
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_ONE, GL_ZERO);

    // -------------------------
    // 12. Desenlazamos el VAO (buena práctica)
    // -------------------------
    glBindVertexArray(0);
}


// -------------------------
// keyPressEvent()
// -------------------------
// Maneja el movimiento del submarino con las teclas W/A/S/D/Up/Down.
// Se llama cada vez que se pulsa una tecla mientras el widget tiene el foco.
// -------------------------
void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
    // Antes de modificar algo, hacemos makeCurrent() para asegurarnos de que el contexto
    // OpenGL de este widget está activo (necesario si hay hilos o varios widgets OpenGL).
    makeCurrent();

    // Según la tecla, movemos la posición del submarino +/- 0.1 unidades en X/Z/Y:
    switch (event->key()) {
    case Qt::Key_W: SubmarinePos.z -= 0.1f; break;
    case Qt::Key_S: SubmarinePos.z += 0.1f; break;
    case Qt::Key_A: SubmarinePos.x -= 0.1f; break;
    case Qt::Key_D: SubmarinePos.x += 0.1f; break;
    case Qt::Key_Up:   SubmarinePos.y += 0.1f; break;
    case Qt::Key_Down: SubmarinePos.y -= 0.1f; break;
    default:
        // Si no es ninguna de las teclas que nos interesan, ignoramos el evento
        event->ignore();
        return;
    }
    // Tras cambiar la posición, pedimos que se redibuje la escena
    update();
}


// -------------------------
// setIntensitatFoc(int valor)
// -------------------------
// Slot conectado a un QSlider que va de 0 a 100. Recibe 'valor', lo normaliza a [0,1]
// asignando intensitatFoc = valor/100.0f y pide un update().
// -------------------------
void MyGLWidget::setIntensitatFoc(int valor)
{
    intensitatFoc = valor / 100.0f;
    update();
}


// -------------------------
// BOTONES DE COLOR Y CONTROL DE FOCUS (0..3)
// -------------------------
// Cada foco tiene dos slots: un “botoColorN()” para abrir QColorDialog y cambiar color,
// y un “controlFocusN()” para alternar encendido/apagado.
// -------------------------

// ------------- Foco 0 -------------
void MyGLWidget::botoColor0()
{
    // QColorDialog espera colores en formato 0..255. Creamos el color inicial a partir del vec3
    QColor inicial(int(colFocusSubVec[0].r * 255),
                   int(colFocusSubVec[0].g * 255),
                   int(colFocusSubVec[0].b * 255));
    // Abrimos el diálogo con “Focus 1” como título
    QColor elegido = QColorDialog::getColor(inicial, this, tr("Color Focus 1"));
    if (!elegido.isValid()) return;  // Si el usuario canceló, salimos

    // Actualizamos colFocusSubVec[0] con los componentes R/G/B en [0,1]
    colFocusSubVec[0] = glm::vec3(elegido.redF(),    // redF() devuelve valor 0.0..1.0
                                  elegido.greenF(),
                                  elegido.blueF());
    update();  // Redibujar la escena para que el color tenga efecto
}

void MyGLWidget::controlFocus0()
{
    focusConectat[0] = !focusConectat[0]; // Alternamos true<->false
    update();
}

// ------------- Foco 1 -------------
void MyGLWidget::botoColor1()
{
    QColor inicial(int(colFocusSubVec[1].r * 255),
                   int(colFocusSubVec[1].g * 255),
                   int(colFocusSubVec[1].b * 255));
    QColor elegido = QColorDialog::getColor(inicial, this, tr("Color Focus 2"));
    if (!elegido.isValid()) return;
    colFocusSubVec[1] = glm::vec3(elegido.redF(), elegido.greenF(), elegido.blueF());
    update();
}

void MyGLWidget::controlFocus1()
{
    focusConectat[1] = !focusConectat[1];
    update();
}

// ------------- Foco 2 -------------
void MyGLWidget::botoColor2()
{
    QColor inicial(int(colFocusSubVec[2].r * 255),
                   int(colFocusSubVec[2].g * 255),
                   int(colFocusSubVec[2].b * 255));
    QColor elegido = QColorDialog::getColor(inicial, this, tr("Color Focus 3"));
    if (!elegido.isValid()) return;
    colFocusSubVec[2] = glm::vec3(elegido.redF(), elegido.greenF(), elegido.blueF());
    update();
}

void MyGLWidget::controlFocus2()
{
    focusConectat[2] = !focusConectat[2];
    update();
}

// ------------- Foco 3 -------------
void MyGLWidget::botoColor3()
{
    QColor inicial(int(colFocusSubVec[3].r * 255),
                   int(colFocusSubVec[3].g * 255),
                   int(colFocusSubVec[3].b * 255));
    QColor elegido = QColorDialog::getColor(inicial, this, tr("Color Focus 4"));
    if (!elegido.isValid()) return;
    colFocusSubVec[3] = glm::vec3(elegido.redF(), elegido.greenF(), elegido.blueF());
    update();
}

void MyGLWidget::controlFocus3()
{
    focusConectat[3] = !focusConectat[3];
    update();
}
