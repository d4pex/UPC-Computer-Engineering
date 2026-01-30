// MyGLWidget.cpp

#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

// Constructor de MyGLWidget
MyGLWidget::MyGLWidget(QWidget* parent) : BL2GLWidget(parent) {
    // Hacemos que este widget reciba eventos de teclado aun cuando no haya
    // clic previo (Qt::StrongFocus). Así podemos pulsar teclas sin seleccionar antes.
    setFocusPolicy(Qt::StrongFocus);

    // ----------------------------
    // Configuración del temporizador para la puerta roja
    // ----------------------------
    temporitzadorPorta = new QTimer(this);
    // Conectamos la señal timeout() del temporizador a la ranura actualitzarPorta()
    // Esto significa que cada vez que el temporizador se dispare, se llamará a actualitzarPorta()
    connect(temporitzadorPorta, &QTimer::timeout, this, &MyGLWidget::actualitzarPorta);
    // Fijamos el intervalo de tiempo en 100 milisegundos (0.1 s) para animar suavemente
    temporitzadorPorta->setInterval(100);

    // ----------------------------
    // Configuración del temporizador para el movimiento automático de Rick
    // ----------------------------
    temporitzadorRick = new QTimer(this);
    // Conectamos timeout() a moureRick(): cada vez que pasen 100 ms, moveremos a Rick
    connect(temporitzadorRick, &QTimer::timeout, this, &MyGLWidget::moureRick);
    temporitzadorRick->setInterval(100);
}

// Destructor de MyGLWidget
MyGLWidget::~MyGLWidget() {
    // Llamamos explícitamente al destructor de la clase base para limpiar recursos, aunque Qt lo haría automáticamente.
    BL2GLWidget::~BL2GLWidget();
}

// initializeGL() se llama una vez al crear el contexto OpenGL
void MyGLWidget::initializeGL() {
    // Llamamos a la inicialización base de BL2GLWidget:
    // - compilación y enlace de shaders
    // - creación de buffers (creaBuffersTerra, creaBuffersCub, creaBuffersModels)
    // - obtención de locations de atributos y uniforms básicos (viewLoc, projLoc, transLoc, etc.)
    BL2GLWidget::initializeGL();

    // Obtenemos la ubicación (location) de la variable uniforme "porta3" en el Vertex Shader.
    // La usaremos luego para indicarle al shader cuándo debe pintar la puerta roja de color rojo.
    porta3Loc = glGetUniformLocation(program->programId(), "porta3");

    // Inicializamos la escena y la cámara
    iniEscena();
    iniCamera();
}

// iniEscena() prepara todas las posiciones, escalas y atributos de la escena inicial
void MyGLWidget::iniEscena() {
    // ----------------------------
    // 1. Calcular atributos de la escena global (centro y radio)
    // ----------------------------
    // Definimos los puntos mínimo y máximo que encierran toda la escena:
    // - puntMin y puntMax abarcan desde (-6,0,-4) hasta (6,2.75,4)
    //   la base del suelo va de -6 a +6 en X y -4 a +4 en Z, con una altura que llega hasta 2.75 en Y.
    glm::vec3 puntMin(-6.0f, 0.0f, -4.0f);
    glm::vec3 puntMax( 6.0f, 2.75f,  4.0f);

    // Calculamos el centro y el radio de la escena basándonos en esas cotas
    calculaCentreRadiEscena(puntMin, puntMax);

    // ----------------------------
    // 2. Inicializar atributos del modelo "Rick"
    // ----------------------------
    // calculaCapsaModel(): dado el modelo (rick) y la altura deseada (1.5),
    // calcula:
    //   - escalaRick: factor de escala para que Rick mida 1.5 unidades de alto
    //   - centreCaixaRick: centro de la base de la caja contenedora de Rick
    calculaCapsaModel(rick, escalaRick, 1.5f, centreCaixaRick);

    // Posicionamos a Rick inicialmente en (-5, 0, 0), con centro de la base en Y=0.
    // Así aparece a la izquierda del escenario mirando hacia +X (más adelante fijamos el ángulo).
    posRick = glm::vec3(-5.0f, 0.0f, 0.0f);

    // orientamos a Rick para que mire hacia +X (ángulo 90 grados = PI/2 radianes)
    angleRick = float(M_PI)/2.0f;

    // ----------------------------
    // 3. Inicializar atributos de las dos puertas azules
    // ----------------------------
    // tamanyPortas define el tamaño xyz de cada puerta: 0.5 (ancho) x 2 (alto) x 3 (fondo)
    tamanyPortas = glm::vec3(0.5f, 2.0f, 3.0f);

    // posicionsPorta[0] será la puerta con centro de la base en (0,0,2.5)
    // posicionsPorta[1] será la puerta con centro de la base en (0,0,-2.5)
    // De esta forma, quedan centradas sobre el eje Z en ±2.5, justo encima del suelo.
    posicionsPorta[0] = glm::vec3(0.0f, 0.0f,  2.5f);
    posicionsPorta[1] = glm::vec3(0.0f, 0.0f, -2.5f);

    // ----------------------------
    // 4. Inicializar atributos de la tercera puerta roja (centrada en (0,0,0))
    // ----------------------------
    // tamanyPorta3: 0.25 (ancho) x 2 (alto) x 2 (fondo)
    tamanyPorta3 = glm::vec3(0.25f, 2.0f, 2.0f);

    // posicioPorta3: centro de la base en (0,0,0) inicialmente
    posicioPorta3 = glm::vec3(0.0f, 0.0f, 0.0f);

    // ----------------------------
    // 5. Inicializar atributos de la videocámara que hace de cámara interna
    // ----------------------------
    // calculaCapsaModel(): para el modelo videoCamera, la altura será 0.5
    calculaCapsaModel(videoCamera, escalaVideoCamera, 0.5f, centreCaixaVideoCamera);

    // Posicionamos la videocámara en (0, 2.25, -1). El valor Y=2.25 asegura que esté elev
    // un poco por encima del suelo apuntando hacia Rick.
    posVideoCamera = glm::vec3(0.0f, 2.25f, -1.0f);

    // Inicialmente no rota (ángulo 0): se actualizará dinámicamente para mirar a Rick.
    angleVideoCamera = 0.0f;
}

// iniCamera() configura los parámetros iniciales de la cámara externa (tercera persona)
// y envía las primeras matrices de vista y proyección
void MyGLWidget::iniCamera() {
    // dist: colocamos la cámara externa a una distancia igual a 2 veces el radio de la escena
    dist = radiEscena * 2.0f;

    // fovInicial: el ángulo fov tal que la escena (de radio radiEscena) ocupe el máximo
    // posible del viewport sin recortarse. Calculamos con asin(radio/dist)*2.
    fovInicial = 2.0f * asin(radiEscena / dist);

    // Partimos con fov = fovInicial (la cámara externa arranca con ese fov)
    fov = fovInicial;

    // znear y zfar para la cámara externa: znear a dist - radiEscena, zfar a dist + radiEscena
    // De esta forma, todo el contenido de la escena entre centro-radiEscena y centro+radiEscena queda dentro del frustum
    znear = dist - radiEscena;
    zfar  = dist + radiEscena;

    // Ángulos iniciales de Euler (phi = inclinación, theta = azimut)
    // Se pide que inicialmente sean M_PI/4, M_PI/4
    phi   = M_PI / 4.0f;
    theta = M_PI / 4.0f;

    // Iniciamos con la cámara externa (cameraExterna = true)
    cameraExterna = true;

    // Enviamos la vista y proyección iniciales
    viewTransform();
    projectTransform();
}

// Cuando se redimensiona la ventana, recalculamos el aspecto (ra), el FOV en caso necesario
// y actualizamos las matrices de vista y proyección para evitar deformaciones.
void MyGLWidget::resizeGL(int w, int h) {
    // ra = ratio de aspecto ancho/alto
    ra = float(w) / float(h);

    if (cameraExterna) {
        // Si la cámara externa está activa:
        // Si ra < 1 (ventana más alta que ancha), aumentamos FOV vertical para no recortar
        if (ra < 1.0f) {
            // fov nuevo = 2 * atan( tan(fovInicial/2) / ra )
            // Ajusta fov para que la escena no se recorte en pantallas altas
            fov = 2.0f * atan(tan(0.5f * fovInicial) / ra);
        } else {
            // Si ra >= 1, seguimos con fovInicial
            fov = fovInicial;
        }
    } else {
        // Si estamos en cámara interna:
        if (ra < 1.0f) {
            // Ajustamos fov interno (fovInterna = PI/2) si la ventana es alta
            fov = 2.0f * atan(tan(0.5f * fovInterna) / ra);
        } else {
            // Si ra >= 1, mantenemos fovInterna
            fov = fovInterna;
        }
    }

    // Reenviamos las transformaciones actualizadas
    viewTransform();
    projectTransform();

    // Indicamos a OpenGL que el viewport abarca toda la ventana (desde 0,0 hasta w,h)
    glViewport(0, 0, w, h);
}

// viewTransform(): calcula y envía la matriz de vista (View) al Vertex Shader
void MyGLWidget::viewTransform() {
    // Partimos de la matriz identidad
    glm::mat4 View = glm::mat4(1.0f);

    if (cameraExterna) {
        // Cámara externa (inspección con ángulos phi/theta):
        // 1) Trasladamos la cámara en -Z la distancia dist
        View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -dist));
        // 2) Rotamos en X por phi (inclinación vertical)
        View = glm::rotate(View, phi, glm::vec3(1.0f, 0.0f, 0.0f));
        // 3) Rotamos en Y por -theta (azimut horizontal)
        View = glm::rotate(View, -theta, glm::vec3(0.0f, 1.0f, 0.0f));
        // 4) Finalmente, trasladamos para que el centro de la escena quede en el origen de la vista
        View = glm::translate(View, -centreEscena);
    } else {
        // Cámara interna (videocámara fija mirando a Rick):
        // obs = posición de la videocámara
        glm::vec3 obs = posVideoCamera;
        // vrp (view reference point) = posición de Rick, queremos mirar hacia allí
        glm::vec3 vrp = posRick;
        // up = (0,1,0) para indicar que el "arriba" es el eje Y positivo
        glm::vec3 up  = glm::vec3(0, 1, 0);
        // glm::lookAt genera la matriz de vista automáticamente
        View = glm::lookAt(obs, vrp, up);
    }

    // Enviamos la matriz View al shader en la variable uniform "View" (ubicación viewLoc)
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

// projectTransform(): calcula y envía la matriz de proyección (Proj) al Vertex Shader
void MyGLWidget::projectTransform() {
    // Partimos de la identidad
    glm::mat4 Proj;

    if (cameraExterna) {
        // Para cámara externa usamos perspectiva con fov calculado, ratio ra, znear/zfar
        Proj = glm::perspective(fov, ra, znear, zfar);
    } else {
        // Para cámara interna (videocámara), usamos:
        // - fovInterna (PI/2)
        // - ratio ra
        // - znear muy pequeño (0.1) para no recortar objetos cercanos
        // - zfar = 8.0f (suficiente para abarcar toda la escena desde la videocámara)
        Proj = glm::perspective(fov, ra, 0.1f, 8.0f);
    }

    // Enviamos la matriz Proj al shader en la variable uniform "Proj" (ubicación projLoc)
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

// rickTransform(): construye la matriz de modelo para Rick y la envía al shader
void MyGLWidget::rickTransform() {
    // Partimos de la identidad
    glm::mat4 TG(1.0f);
    // 1) Trasladamos Rick a su posición posRick (centro base en xyz)
    TG = glm::translate(TG, posRick);
    // 2) Rotamos según angleRick alrededor del eje Y (para que mire en la dirección adecuada)
    TG = glm::rotate(TG, angleRick, glm::vec3(0, 1, 0));
    // 3) Escalamos uniformemente con factor escalaRick (para que mida la altura deseada)
    TG = glm::scale(TG, glm::vec3(escalaRick));
    // 4) Para centrar el modelo, trasladamos -centreCaixaRick (restamos el centro de la base)
    TG = glm::translate(TG, -centreCaixaRick);

    // Enviamos la matriz TG (Transformación del modelo) al shader en "TG" (ubicación transLoc)
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

// videoCameraTransform(): construye la matriz de modelo para la videocámara y la envía al shader
void MyGLWidget::videoCameraTransform() {
    glm::mat4 TG(1.0f);
    // 1) Trasladamos a posVideoCamera
    TG = glm::translate(TG, posVideoCamera);
    // 2) Rotamos según angleVideoCamera (aunque en este ejercicio estático casi no cambia)
    TG = glm::rotate(TG, angleVideoCamera, glm::vec3(0, 1, 0));
    // 3) Escalamos con factor escalaVideoCamera (altura = 0.5)
    TG = glm::scale(TG, glm::vec3(escalaVideoCamera));
    // 4) Trasladamos para centrar base: -centreCaixaVideoCamera
    TG = glm::translate(TG, -centreCaixaVideoCamera);

    // Enviamos TG al shader
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

// portaTransform(): construye la matriz de modelo para cualquiera de las puertas (azules o roja)
// pos: centro de la base; mida: dimensiones en x,y,z
void MyGLWidget::portaTransform(const glm::vec3 &pos, const glm::vec3 &mida) {
    glm::mat4 TG(1.0f);
    // 1) Trasladar al centro de la base
    TG = glm::translate(TG, pos);
    // 2) Escalar según las dimensiones indicadas
    TG = glm::scale(TG, mida);

    // Enviamos TG al shader para que el cubo (unitario en VAO_Cub) quede con esas dimensiones
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

// paintGL(): se llama cada vez que toca redibujar la escena completa
void MyGLWidget::paintGL() {
    makeCurrent(); // Nos aseguramos de usar el contexto actual

    // Borramos color y buffer de profundidad antes de dibujar
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reenviamos las transformaciones por si han cambiado
    projectTransform();
    viewTransform();

    // ----------------------------------------
    // 1. Pintar a Rick
    // ----------------------------------------
    // Asociamos el VAO de Rick (VAO_Rick), creado en BL2GLWidget::creaBuffersModels()
    glBindVertexArray(VAO_Rick);
    // Aplicamos la transformación de Rick
    rickTransform();
    // Dibujamos todos los triángulos (cada cara del modelo tiene 3 vértices)
    glDrawArrays(GL_TRIANGLES, 0, rick.faces().size() * 3);

    // ----------------------------------------
    // 2. Pintar la videocámara (solo cuando estamos en cámara externa)
    // ----------------------------------------
    if (cameraExterna) {
        glBindVertexArray(VAO_VideoCamera);
        videoCameraTransform();
        glDrawArrays(GL_TRIANGLES, 0, videoCamera.faces().size() * 3);
    }

    // ----------------------------------------
    // 3. Pintar las dos puertas azules
    // ----------------------------------------
    glBindVertexArray(VAO_Cub); // VAO_Cub contiene los vértices de un cubo unitario
    for (int i = 0; i < 2; ++i) {
        // Para cada una de las dos puertas azules:
        portaTransform(posicionsPorta[i], tamanyPortas);
        // Cada puerta es un cubo de 36 vértices (12 triángulos)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // ----------------------------------------
    // 4. Control de apertura/cierre de la puerta roja (puerta3)
    // ----------------------------------------
    // Si Rick está entre X = -2 y X = 2, abrimos la puerta (porta3Oberta = true)
    if (posRick.x <= 2.0f && posRick.x >= -2.0f) {
        porta3Oberta = true;
    }
    // Si Rick está fuera de ese rango, cerramos la puerta
    else if (posRick.x > 2.0f || posRick.x < -2.0f) {
        porta3Oberta = false;
    }
    // Llamamos a mourePorta() para que comience el movimiento si es necesario
    mourePorta(porta3Oberta);

    // ----------------------------------------
    // 5. Pintar la puerta roja
    // ----------------------------------------
    glBindVertexArray(VAO_Cub);
    // Antes de la transformación, le pasamos al shader que "porta3" = true
    // Esto hará que en el Vertex Shader fcolor = vec3(1,0,0) (rojo)
    glUniform1i(porta3Loc, true);
    portaTransform(posicioPorta3, tamanyPorta3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // Una vez pintada, volvemos a poner "porta3" = false para que no afecte a otros dibujos
    glUniform1i(porta3Loc, false);

    // ----------------------------------------
    // 6. Pintar el suelo (VAO_Terra)
    // ----------------------------------------
    glBindVertexArray(VAO_Terra);
    // identTransform(): simplemente envía la matriz identidad a "TG"
    identTransform();
    // El suelo se dibuja con GL_TRIANGLE_STRIP y 4 vértices
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Desvinculamos cualquier VAO activo
    glBindVertexArray(0);
}

// mouseMoveEvent(): se dispara cuando movemos el ratón con un botón pulsado.
// Sirve para rotar la cámara externa si DoingInteractive == ROTATE.
void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
    makeCurrent(); // Contexto activo

    // Solo rotamos si estamos en cámara externa y el modo interactivo es ROTATE
    if (cameraExterna && DoingInteractive == ROTATE) {
        // Al arrastrar, calculamos cuanto cambió el ratón en X y en Y (relativo a la última posición)
        // e->x() - xClick: diferencia en pixels horizontales
        // Multiplicamos por 0.005f para convertir pixels a radianes de ángulo
        theta -= (e->x() - xClick) * 0.005f;  // moviendo a la derecha, theta aumenta => giramos escena a la derecha
        phi   += (e->y() - yClick) * 0.005f;  // moviendo hacia arriba, phi aumenta => giramos escena hacia abajo

        // Actualizamos la vista con los nuevos phi y theta
        viewTransform();
    }
    // Guardamos la posición actual del ratón para el próximo movimiento
    xClick = e->x();
    yClick = e->y();

    // Llamamos a update() para que se redibuje la escena con la nueva vista
    update();
}

// keyPressEvent(): captura las pulsaciones de teclado
void MyGLWidget::keyPressEvent(QKeyEvent* event) {
    makeCurrent(); // Contexto activo

    // switch sobre la tecla pulsada
    switch (event->key()) {
    case Qt::Key_Up:
        // Flecha arriba: movimiento manual de Rick hacia +X (una unidad)
        // Solo si no hay movimiento automático en curso (movimentAutomatic == false)
        if (!movimentAutomatic && posRick.x + 1.0f <= 6.0f) {
            posRick.x += 1.0f;      // movemos 1 unidad en +X
            direccioRick = +1;      // dirección = +1 (para la animación automática)
            angleRick = M_PI / 2.0f; // rotamos a Rick para que mire hacia +X (90º)
        }
        break;

    case Qt::Key_Down:
        // Flecha abajo: movimiento manual de Rick hacia -X
        if (!movimentAutomatic && posRick.x - 1.0f >= -6.0f) {
            posRick.x -= 1.0f;        // movemos 1 unidad en -X
            direccioRick = -1;        // dirección = -1
            angleRick = 3 * M_PI / 2.0f; // rotamos a Rick para que mire hacia -X (270º)
        }
        break;

    case Qt::Key_C:
        // Tecla C: alternar entre cámara externa e interna
        cameraExterna = !cameraExterna;
        // Una vez cambiada la cámara, actualizamos matrices y viewport
        viewTransform();
        projectTransform();
        resizeGL(width(), height());
        update();
        break;

    case Qt::Key_R:
        // Tecla R: resetear la escena a su estado inicial
        resetEscena();
        break;

    case Qt::Key_A:
        // Tecla A: alternar movimiento automático de Rick
        alternarMovimentAutomatic();
        break;

    default:
        // Para cualquier otra tecla, delegamos a la implementación base (por ejemplo, Quitar clipping planes, etc.)
        BL2GLWidget::keyPressEvent(event);
        break;
    }

    // Finalmente, pedimos actualizar la ventana
    update();
}

// calculaCentreRadiEscena(): a partir de puntMin y puntMax calcula centroEscena y radiEscena
void MyGLWidget::calculaCentreRadiEscena(glm::vec3 puntMin, glm::vec3 puntMax) {
    // Centro = punto medio entre puntMin y puntMax
    centreEscena = (puntMin + puntMax) / 2.0f;

    // half = vector desde el centro hasta puntMax
    glm::vec3 half = puntMax - centreEscena;
    // radio = distancia euclídea desde el centro hasta ese punto máximo
    radiEscena = sqrt(half.x * half.x + half.y * half.y + half.z * half.z);
}

// canviarCamera(): slot que también alterna cámara externa/interna (similar a pulsar C)
void MyGLWidget::canviarCamera() {
    makeCurrent();
    cameraExterna = !cameraExterna;
    viewTransform();
    resizeGL(width(), height());
    update();
}

// resetEscena(): slot que deja todo EXACTAMENTE como al inicio
void MyGLWidget::resetEscena() {
    // Volvemos a inicializar los parámetros de escena y cámara
    iniEscena();
    iniCamera();
    // Ajustamos viewport para el tamaño actual
    resizeGL(width(), height());
    update();
}

// mourePorta(): inicia el movimiento de la puerta roja hacia "abierta" o "cerrada"
void MyGLWidget::mourePorta(bool obrir) {
    // Si ya está en movimiento, no hacemos nada para evitar reinicios bruscos
    if (portaEnMoviment) return;

    // Guardamos posición inicial de la puerta
    iniciPorta = posicioPorta3;

    if (obrir) {
        // Si queremos abrir, el destino será mover la puerta hasta (0,0,2)
        destiPorta = glm::vec3(0.0f, 0.0f, 2.0f);
    } else {
        // Si queremos cerrar, el destino vuelve a ser la posición inicial (0,0,0)
        destiPorta = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    // Velocidad del desplazamiento (unidades por tick)
    velocitatPorta = 0.2f;
    // Indicamos que la puerta está en movimiento
    portaEnMoviment = true;
    // Iniciamos el temporizador para que cada 100 ms se llame a actualitzarPorta()
    temporitzadorPorta->start();
}

// actualitzarPorta(): slot llamado cada vez que temporitzadorPorta hace timeout (100 ms)
void MyGLWidget::actualitzarPorta() {
    // Calculamos vector dirección entre la posición actual y la de destino
    glm::vec3 direccion = destiPorta - posicioPorta3;
    // Calculamos la distancia euclídea restante
    float distancia = glm::length(direccion);

    if (distancia < velocitatPorta) {
        // Si la distancia restante es menor que el paso (velocitatPorta),
        // directamente ponemos la puerta en la posición de destino
        posicioPorta3 = destiPorta;
        // Detenemos el movimiento de la puerta
        aturarMovimentPorta();
    } else {
        // Si aún no llegó, normalizamos el vector dirección y movemos la puerta
        if (distancia > 0.0f) {
            direccion = direccion / distancia; // vector unitario
        }
        // Avanzamos unas décimas en la dirección adecuada
        posicioPorta3 += direccion * velocitatPorta;
    }

    // Pedimos redibujar la escena para ver la puerta en nueva posición
    update();
}

// aturarMovimentPorta(): detiene el temporizador cuando la puerta ha llegado
void MyGLWidget::aturarMovimentPorta() {
    portaEnMoviment = false;      // indicamos que ya no está en movimiento
    temporitzadorPorta->stop();   // detenemos el temporizador
}

// alternarMovimentAutomatic(): slot para activar/desactivar el movimiento automático de Rick
void MyGLWidget::alternarMovimentAutomatic() {
    // Cambiamos el estado:
    movimentAutomatic = !movimentAutomatic;

    if (movimentAutomatic) {
        // Si acabamos de activar el movimiento automático, iniciamos el timer
        temporitzadorRick->start();
    } else {
        // Si acabamos de desactivarlo, detenemos el timer
        temporitzadorRick->stop();
    }
    update();
}

// moureRick(): slot llamado por temporitzadorRick cada 100 ms si el movimiento automático está activo
void MyGLWidget::moureRick() {
    if (!movimentAutomatic) return; // si no está activado, no hacemos nada

    // Desplazamos a Rick en X en direccioRick * velocitatRick
    posRick.x += direccioRick * velocitatRick;

    // Si toca el límite derecho (>= +6), ponemos exactamente en +6 y cambiamos dirección
    if (posRick.x >=  6.0f) {
        posRick.x     = 6.0f;
        direccioRick = -1; // invertimos para que vuelva
    }
    // Si toca el límite izquierdo (<= -6), ponemos en -6 y cambiamos dirección
    else if (posRick.x <= -6.0f) {
        posRick.x     = -6.0f;
        direccioRick = +1; // invertimos para que vaya a la derecha
    }

    // Ajustamos la rotación de Rick para que mire siempre en la dirección de avance:
    if (direccioRick > 0) {
        angleRick = float(M_PI / 2.0f);      // 90° para mirar a +X
    } else {
        angleRick = float(3.0f * M_PI / 2.0f); // 270° para mirar a -X
    }

    // Redibujamos la escena
    update();
}
