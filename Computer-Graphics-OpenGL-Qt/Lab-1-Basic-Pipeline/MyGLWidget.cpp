//#include <GL/glew.h>  // Comentado porque no es necesario si usamos QOpenGLFunctions
#include "MyGLWidget.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"  // Para funciones de transformación (translate, rotate, scale)
#include <QTimer>                         // Para crear temporizadores que llamen a tick()
#include "MyForm.h"                       // Asumimos que existe una clase MyForm con constantes (ej: CLOCK_PERIOD_MILIS)

#include <iostream> // Para imprimir mensajes de depuración si fuera necesario

// Constructor de MyGLWidget.
// Se inicializa QOpenGLWidget (pasa parent) y se inicializa 'program' a nullptr.
MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
    // Para que este widget reciba eventos de teclado, hay que indicarlo explícitamente:
    setFocusPolicy(Qt::StrongFocus);
}

// Destructor: libera memoria de 'program' si se había creado.
MyGLWidget::~MyGLWidget ()
{
    if (program != NULL)
        delete program;
}

// Slot tick(): se llama periódicamente (por ejemplo, cada 16 ms).
// Incrementa la variable de tiempo 't' y emite la señal timeUpdate.
// Luego pide que se vuelva a dibujar (update()).
void MyGLWidget::tick(){
    // MyForm::CLOCK_PERIOD_MILIS / 1000.0f = incremento en segundos (si CLOCK_PERIOD_MILIS = 16, incrementa ~0.016 s)
    t += MyForm::CLOCK_PERIOD_MILIS / 1000.0f;
    // Emitimos la señal para actualizar cualquier widget que muestre el tiempo
    emit timeUpdate(t);
    // Forzamos que Qt llame a paintGL() lo antes posible
    update();
}

// Pone el tiempo 't' a cero y emite la señal para actualizar en la GUI.
void MyGLWidget::resetTime(){
    t = 0;
    emit timeUpdate(t);
}

// Devuelve el valor actual de la variable de tiempo 't'
float MyGLWidget::getTime(){
    return t;
}

// initializeGL(): se llama una vez al crear el contexto OpenGL.
// Aquí inicializamos todo lo relacionado con shaders, buffers, estados iniciales de OpenGL, etc.
void MyGLWidget::initializeGL ()
{
    // Esta llamada carga los punteros de las funciones OpenGL 3.3 Core
    initializeOpenGLFunctions();

    // Establecemos el color de fondo (clear color) en un tono celeste claro.
    glClearColor(200/255.0f, 220/255.0f, 255/255.0f, 1.0f);

    // Compilamos y enlazamos los shaders vertex + fragment
    carregaShaders();
    // Creamos todos los VAO/VBO necesarios para los objetos de la escena
    creaBuffers();

    // Aquí podríamos inicializar otras variables de estado si hiciera falta.
}

// paintGL(): se llama cada vez que Qt necesita redibujar el widget.
// Aquí se hace todo el renderizado de la escena: limpiar buffers, calcular animaciones y dibujar objetos.
void MyGLWidget::paintGL ()
{
// Código adicional para MacOS con pantallas retina, donde el viewport puede tener dimensiones diferentes.
#ifdef __APPLE__
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    ample = vp[2];
    alt = vp[3];
#endif

    // Limpiamos el buffer de color (la pantalla) con el clear color definido antes.
    glClear(GL_COLOR_BUFFER_BIT);

    // Dibujamos la base (suelo) de la escena
    pintaTerra();

    // Definimos los desplazamientos en X de cada uno de los 4 péndulos.
    // Cada bola tiene diámetro 0.25, así que se colocan separadas 0.25 unidades.
    // La base (soporte superior) empieza en x = -0.5, pero hay que sumar el radio 0.125 para colocar el centro de la bola.
    float offsets_X[4] = {-0.375f, -0.125f, 0.125f, 0.375f};

    // Comprobamos si estamos en modo manual o automático
    if (esModeManual()) { // MODO MANUAL
        // Si antes estábamos en modo automático, desactivamos la bandera para no re-inicializar la siguiente vez
        if (canviAutomatic) {
            canviAutomatic = false;
        }

        // Restauramos los ángulos guardados por tecla para péndulo1 y péndulo4
        angles[0] = angleP1;
        angles[3] = angleP4;

        // Para evitar que el widget pierda el foco de teclado al cambiar de modo,
        // se llama a grabKeyboard() para asegurarse de seguir recibiendo eventos.
        grabKeyboard();
    }
    else { // MODO AUTOMÁTICO
        // Obtenemos el tiempo transcurrido desde que se activó el modo automático
        float temps = this->getTime();

        // Si acabamos de cambiar a modo automático, inicializamos la configuración:
        // - solo una vez, ponemos péndulo1 en -40° y péndulo4 en 0°
        if (!canviAutomatic) {
            canviAutomatic = true;  // indicamos que ya pasamos por esta inicialización
            faseAuto = 1;           // empezamos en fase 1 (mover péndulo 1)
            angles[0] = -40.0f;     // ángulo inicial del péndulo 1
            angles[3] = 0.0f;       // ángulo inicial del péndulo 4
        }

        // Gestión por fases del oscilamiento:
        switch (faseAuto) {
        case 1:
            // Fase 1: el péndulo 1 oscila con coseno de frecuencia 4 rad/s
            angles[0] = -40.0f * cos(temps * 4.0f);
            // Cuando alcance un ángulo positivo, lo forzamos a 0 y pasamos a fase 2
            if (angles[0] > 0.0f) {
                angles[0] = 0.0f;
                faseAuto = 2;
            }
            break;

        case 2:
            // Fase 2: el péndulo 4 oscila igual que el péndulo 1
            angles[3] = -40.0f * cos(temps * 4.0f);
            // Cuando alcance ángulo ≤ 0, lo forzamos a 0 y volvemos a fase 1
            if (angles[3] <= 0.0f) {
                angles[3] = 0.0f;
                faseAuto = 1;
            }
            break;

        default:
            break;
        }
    }

    // Ahora que tenemos el array 'angles' actualizado (ya sea por tecla o automáticamente),
    // pintamos los cuatro péndulos consecutivamente:
    for (int i = 0; i < 4; ++i) {
        pintaBola(offsets_X[i], angles[i]);
        pintaEstrella();           // se dibuja la estrella en cada iteración (se superpone)
        pintaSuport(offsets_X[i], angles[i]);
    }

    // Desvinculamos cualquier VAO que pudiera quedar activo
    glBindVertexArray(0);
}

// resizeGL(): se llama cuando el widget cambia de tamaño.
// Aquí guardamos en 'ample' y 'alt' las dimensiones en píxeles.
void MyGLWidget::resizeGL(int w, int h)
{
    ample = w;
    alt = h;
}

// ------------------- RUTINAS DE DIBUJO para cada objeto -------------------

// Dibuja la bola (círculo) de cada péndulo en la posición desplazada 'd' con rotación 'angle'
void MyGLWidget::pintaBola(float d, float angle) {
    // Primero enviamos la matriz de transformación calculada en transformacioBola
    transformacioBola(d, angle);
    // Vinculamos el VAO que contiene los vértices y colores de un círculo
    glBindVertexArray(VAO_BOLA);
    // Dibujamos con GL_TRIANGLE_FAN, usando CIRCLE_SAMPLES+1 vértices
    glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_SAMPLES + 1);
}

// Dibuja la estrella (figura estática, no recibe parámetros de posición/ángulo)
void MyGLWidget::pintaEstrella() {
    glBindVertexArray(VAO_ESTRELLA);
    // Usamos GL_TRIANGLE_FAN con 12 vértices para cerrar la estrella
    glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
}

// Dibuja el soporte (barra) de cada péndulo en posición d y rotación angulo
void MyGLWidget::pintaSuport(float d, float angle) {
    transformacioSuport(d, angle);
    glBindVertexArray(VAO_SUPORT);
    // GL_TRIANGLE_STRIP para una tira rectangular de 4 vértices que representa la barra
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// Dibuja la base / suelo (una superficie con efecto de bandas de color)
// Usa color y uniform “aplicarEfecto” para alternar patrón de franjas
void MyGLWidget::pintaTerra() {
    transformacioTerra();  // Redefinimos la matriz a identidad (no se desplaza)
    // Activamos el programa de shader
    glUseProgram(program->programId());
    // Indicamos al fragment shader que aplique el efecto (uniform bool aplicarEfecto = true)
    glUniform1i(glGetUniformLocation(program->programId(), "aplicarEfecto"), true);
    // Pasamos el color marrón (uniform vec3 COLOR_MARRO)
    glUniform3fv(glGetUniformLocation(program->programId(), "COLOR_MARRO"), 1, &COLOR_MARRO[0]);
    // Vinculamos el VAO de la tierra y dibujamos 6 vértices como TRIANGLE_STRIP
    glBindVertexArray(VAO_TERRA);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    // Desactivamos el efecto para que los siguientes objetos se dibujen con su color propio
    glUniform1i(glGetUniformLocation(program->programId(), "aplicarEfecto"), false);
}

// Maneja eventos de pulsación de teclas únicamente en modo manual.
// Cambia los ángulos angleP1 y angleP4 según las teclas A, D, Q, E.
void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
    // Si no estamos en modo manual, ignoramos las teclas
    if (!esModeManual()) {
        return;
    }

    // Antes de actualizar, hacemos que el contexto actual sea este widget
    makeCurrent();

    switch (event->key()) {
    case Qt::Key_A: {
        // Tecla 'A': decrementar ángulo de péndulo 1 en 5°, límite mínimo -40°
        if (angleP1 > -40.0f) {
            angleP1 -= 5.0f;
        }
        break;
    }
    case Qt::Key_D: {
        // Tecla 'D': incrementar ángulo de péndulo 1 en 5°, límite máximo 0°
        if (angleP1 < 0.0f) {
            angleP1 += 5.0f;
        }
        break;
    }
    case Qt::Key_Q: {
        // Tecla 'Q': decrementar ángulo de péndulo 4 en 5°, límite mínimo 0°
        if (angleP4 > 0.0f) {
            angleP4 -= 5.0f;
        }
        break;
    }
    case Qt::Key_E: {
        // Tecla 'E': incrementar ángulo de péndulo 4 en 5°, límite máximo 40°
        if (angleP4 < 40.0f) {
            angleP4 += 5.0f;
        }
        break;
    }
    default:
        // Para cualquier otra tecla, dejamos que Qt procese el evento normalmente
        event->ignore();
        break;
    }

    // Una vez cambiados los ángulos, pedimos repintado
    update();
}

// ---------------- Transformaciones -----------------

// Para la base (suelo) no hay transformación: usamos la matriz identidad
void MyGLWidget::transformacioTerra() {
    glm::mat4 transform(1.0f);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

// Transformación de la bola del péndulo:
// 1) Calculamos coordenadas (x,y) según longitud L y ángulo.
// 2) Translación al punto (x,y).
// 3) Rotación alrededor del centro de la bola (eje Z).
// 4) Escalado para que el diámetro sea 0.25 (radio=0.125).
void MyGLWidget::transformacioBola(float d, float angle) {
    glm::mat4 transform(1.0f);

    // Longitud total desde el eje de giro hasta el centro de la bola:
    // columna vertical de soporte (0.5) + radio de la bola (0.125) = 0.625
    float L = 0.625f;
    // Coordenada X = desplazamiento base + L * sin(angulo)
    float x = d + L * sin(glm::radians(angle));
    // Coordenada Y = altura inicial (0.5) - L * cos(angulo)
    float y = 0.5f - L * cos(glm::radians(angle));

    // 1) Trasladamos la bola al punto (x,y)
    transform = glm::translate(transform, glm::vec3(x, y, 0.0f));
    // 2) Rotamos la bola alrededor de Z según el ángulo (para “girar” el círculo, aunque no es estrictamente necesario)
    transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    // 3) Escalamos uniformemente (0.25) para que el diámetro sea 0.25 unidades en el mundo
    transform = glm::scale(transform, glm::vec3(0.25f, 0.25f, 0.25f));

    // Enviamos la matriz resultante al shader (uniform "TG")
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

// Transformación del soporte (barra) del péndulo:
// 1) Traslado inicial para posicionar la parte superior del soporte en (d, 0.5).
// 2) Rotación alrededor del eje Z según el ángulo.
// 3) Traslado de -0.25 en Y para que la barra “gire” correctamente alrededor de su extremo superior.
// 4) Escalado (0.5 en X y Y) para que la barra mida 0.08 x 1.0 (aprox), centrada en su eje.
void MyGLWidget::transformacioSuport(float d, float angle) {
    glm::mat4 transform(1.0f);
    // 1) Colocar la parte superior del soporte en la coordenada correcta
    transform = glm::translate(transform, glm::vec3(d, 0.5f, 0.0f));
    // 2) Rotar la barra según el ángulo alrededor de Z
    transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    // 3) Trasladar -0.25 para mover el centro del soporte al origen antes de escalar
    transform = glm::translate(transform, glm::vec3(0.0f, -0.25f, 0.0f));
    // 4) Escalar en X e Y para que mida 0.08 de ancho y 0.5 de alto
    transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

// ---------------- Creación de buffers (VAO/VBO) -----------------

// Llama a cada función que crea los VAO/VBOs específicos para cada objeto.
// Al final, desactiva el VAO con glBindVertexArray(0).
void MyGLWidget::creaBuffers() {
    creaBufferBola();
    creaBufferSuport();
    creaBufferTerra();
    creaBufferEstrella();
    // Desactivar cualquier VAO activo
    glBindVertexArray(0);
}

// Función genérica que, dado un array de colores y un array de vértices (ambos glm::vec3),
// crea dos VBOs: uno para vértices y otro para colores, y los vincula al VAO activo.
void MyGLWidget::createVBOs(int size, glm::vec3 Colors[], glm::vec3 Vertices[]) {
    // 1) Generamos dos IDs de buffer (VBO[0] para vértices, VBO[1] para colores)
    GLuint VBO[2];
    glGenBuffers(2, VBO);

    // 2) Buffer de vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, size, Vertices, GL_STATIC_DRAW);
    // Configuramos el atributo “vertex” (location = vertexLoc), 3 componentes tipo float.
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // 3) Buffer de colores
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, size, Colors, GL_STATIC_DRAW);
    // Configuramos el atributo “color” (location = colorLoc), 3 componentes tipo float.
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    // 4) Desvinculamos el VAO (no es obligatorio, pero es buena práctica)
    glBindVertexArray(0);
}

// ---------------- Creación de cada VAO/VBO -----------------

// Crea VAO_TERRA: 6 vértices que forman un TRIANGLE_STRIP para la base/“suelo”.
// Cada vértice se coloriza con COLOR_MARRO.
void MyGLWidget::creaBufferTerra() {
    glm::vec3 Vertices[6];
    // Definimos coordenadas de 6 puntos:
    Vertices[0] = glm::vec3(-1.00f,  0.75f,  0.0f);
    Vertices[1] = glm::vec3(-0.50f,  0.50f,  0.0f);
    Vertices[2] = glm::vec3(-1.00f,  1.00f,  0.0f);
    Vertices[3] = glm::vec3( 0.50f,  0.50f,  0.0f);
    Vertices[4] = glm::vec3( 1.00f,  1.00f,  0.0f);
    Vertices[5] = glm::vec3( 1.00f,  0.75f,  0.0f);

    // Generamos un VAO para la base
    glGenVertexArrays(1, &VAO_TERRA);
    glBindVertexArray(VAO_TERRA);

    // Creamos un array de 6 colores idénticos (COLOR_MARRO)
    glm::vec3 Colors[6];
    for(int i = 0; i < 6; i++) {
        Colors[i] = COLOR_MARRO;
    }

    // Creamos los VBOs para vértices y colores, pasándoles el tamaño (sizeof(Vertices))
    createVBOs(sizeof(Vertices), Colors, Vertices);
}

// Crea VAO_SUPORT: 4 vértices para un rectángulo (barra) centrado en el origen.
// Coloreado con COLOR_BLAU (azul).
void MyGLWidget::creaBufferSuport() {
    glm::vec3 Vertices[4];
    // Definimos coordenadas de un rectángulo vertical estrecho (0.08 x 1.0):
    Vertices[0] = glm::vec3(+0.04f, -0.50f, 0.0f);
    Vertices[1] = glm::vec3(-0.04f, -0.50f, 0.0f);
    Vertices[2] = glm::vec3(+0.04f, +0.50f, 0.0f);
    Vertices[3] = glm::vec3(-0.04f, +0.50f, 0.0f);

    // Creamos array de 4 colores idénticos (COLOR_BLAU)
    glm::vec3 Colors[4];
    for(int i = 0; i < 4; i++) {
        Colors[i] = COLOR_BLAU;
    }

    // Generamos y vinculamos el VAO para el soporte
    glGenVertexArrays(1, &VAO_SUPORT);
    glBindVertexArray(VAO_SUPORT);

    createVBOs(sizeof(Vertices), Colors, Vertices);
}

// Crea VAO_BOLA: aproximación de un círculo con CIRCLE_SAMPLES+1 vértices.
// El primer vértice es el centro (0,0). Luego, se generan puntos en el perímetro.
// También se crea un degradado de color amarillo (HSV variando en V).
void MyGLWidget::creaBufferBola() {
    glm::vec3 Colors[CIRCLE_SAMPLES+1];
    glm::vec3 Vertices[CIRCLE_SAMPLES+1];

    // Primer vértice: centro del círculo
    Vertices[0] = glm::vec3(0.0f, 0.0f, 0.0f);
    Colors[0]   = glm::vec3(1.0f, 1.0f, 1.0f); // Color blanco en el centro

    // Configuramos un QColor para obtener componentes HSV
    QColor q(244, 247, 17); // Amarillo brillante
    qreal h, s, v, a;
    q.getHsvF(&h, &s, &v, &a); // obtenemos H, S, V, A en [0..1]

    float radi      = 0.5f;                  // radio inicial para el círculo unitario
    float alfa      = 0.0f;                  // ángulo inicial en radianes
    float deltaAlfa = 2.0 * M_PI / (CIRCLE_SAMPLES - 1); // paso angular
    float deltaV    = 1.0f / (CIRCLE_SAMPLES - 1);       // paso para variar "v" en HSV

    for(int k = 1; k <= CIRCLE_SAMPLES; k++, alfa += deltaAlfa) {
        // a) Coordenadas en el perímetro: (radio*cos(alfa), radio*sin(alfa))
        Vertices[k] = glm::vec3(radi * cos(alfa), radi * sin(alfa), 0.0f);
        // b) Definimos color en RGB a partir de HSV actualizado (q.red(), q.green(), q.blue())
        q.setHsvF(h, s, v, a);
        Colors[k] = glm::vec3(q.red()/255.0f, q.green()/255.0f, q.blue()/255.0f);

        // c) Modificamos 'v' para generar un degradado: decrece en la primera mitad, luego aumenta.
        if (k < CIRCLE_SAMPLES / 2) {
            v -= deltaV;
        } else {
            v += deltaV;
        }
    }

    // Creamos y vinculamos VAO para la bola (círculo)
    glGenVertexArrays(1, &VAO_BOLA);
    glBindVertexArray(VAO_BOLA);

    createVBOs(sizeof(Vertices), Colors, Vertices);
}

// Crea VAO_ESTRELLA: 12 vértices para dibujar una estrella de 5 puntas con TRIANGLE_FAN.
// El primer vértice es el centro, los siguientes 10 alternan entre radio grande y pequeño.
// Y el vértice 11 cierra el contorno repitiendo el primer vértice exterior.
void MyGLWidget::creaBufferEstrella() {
    glm::vec3 Colors[12];
    glm::vec3 Vertices[12];

    // Creamos un QColor para definir el color de la estrella (tonos rojizos)
    QColor q(236, 107, 70);
    qreal h, s, v, a;
    q.getHsvF(&h, &s, &v, &a);

    // 1) Primer vértice = centro de la estrella
    Vertices[0] = glm::vec3(0.0f, 0.0f, 0.0f);
    Colors[0]   = glm::vec3(q.redF(), q.greenF(), q.blueF());

    float radi_gran = 0.25f;            // radio de las puntas
    float radi_petit = 0.125f;          // radio de los entrantes
    float angle = M_PI / 2.0f;          // empezamos apuntando hacia arriba (90°)
    float deltaAngle = M_PI / 5.0f;     // 36° en radianes para las 5 puntas

    // 2) Generación de los 10 vértices exteriores (alternando radio grande/pequeño)
    for(int k = 0; k < 10; k++) {
        float radius = (k % 2 == 0) ? radi_gran : radi_petit;
        Vertices[k + 1] = glm::vec3(radius * cos(angle), radius * sin(angle), 0.0f);
        Colors[k + 1]   = glm::vec3(q.redF(), q.greenF(), q.blueF());
        angle += deltaAngle;
    }

    // 3) Cerramos la estrella conectando el último punto con el primero (exterior)
    Vertices[11] = Vertices[1];
    Colors[11]   = Colors[1];

    // 4) Creamos y vinculamos el VAO de la estrella
    glGenVertexArrays(1, &VAO_ESTRELLA);
    glBindVertexArray(VAO_ESTRELLA);
    createVBOs(sizeof(Vertices), Colors, Vertices);
}

// ------------------- Shaders -------------------

void MyGLWidget::carregaShaders()
{
    // 1) Creamos un shader de tipo Fragment
    QOpenGLShader fs(QOpenGLShader::Fragment, this);
    // 2) Creamos un shader de tipo Vertex
    QOpenGLShader vs(QOpenGLShader::Vertex, this);

    // 3) Compilamos cada uno a partir de ficheros GLSL externos
    fs.compileSourceFile("shaders/basicShader.frag");
    vs.compileSourceFile("shaders/basicShader.vert");

    // 4) Creamos el programa que contendrá ambos shaders
    program = new QOpenGLShaderProgram(this);
    // 5) Añadimos al programa el vertex y el fragment
    program->addShader(&fs);
    program->addShader(&vs);

    // 6) Enlazamos (link) el programa final
    program->link();

    // 7) “Bind” para decir a OpenGL que a partir de ahora use este programa
    program->bind();

    // 8) Obtenemos ubicaciones de atributos en el shader compilado:
    //    - “vertex”: recibe las coordenadas XYZ de los vértices
    //    - “color”: recibe los colores RGB de cada vértice
    vertexLoc = glGetAttribLocation(program->programId(), "vertex");
    colorLoc  = glGetAttribLocation(program->programId(), "color");

    // 9) Obtenemos la ubicación de la uniform “TG” (matriz de transformación)
    transLoc  = glGetUniformLocation(program->programId(), "TG");
}
