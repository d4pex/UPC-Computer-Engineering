// MyGLWidget.h

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

// Incluimos la cabecera base BL2GLWidget, que contiene la lógica
// genérica de inicialización de OpenGL, carga de shaders, etc.
#include "BL2GLWidget.h"

// Incluimos la definición de los modelos (Rick, videocámara, cubo, etc.)
#include "model.h"

// Declaramos nuestra clase MyGLWidget, que hereda de BL2GLWidget.
// BL2GLWidget ya implementa mucha funcionalidad OpenGL (buffer de vértices,
// sombreadores, gestión de eventos básicos), y aquí la ampliamos.
class MyGLWidget : public BL2GLWidget {
    Q_OBJECT   // Macro de Qt para habilitar señales y slots en esta clase

public:
    // Constructor y destructor
    // QWidget *parent=0 indica que, por defecto, este widget no tiene padre
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();

protected:
    // Métodos que sobreescriben (override) las funciones virtuales de BL2GLWidget
    // para inicializar escena, cámara, redibujar al cambiar tamaño, etc.

    // Se llama una vez al principio para inicializar OpenGL (contexto, valores por defecto, etc.)
    void initializeGL() override;
    // Inicializamos la escena: cargamos posiciones, tamaños y escalas iniciales de modelos
    void iniEscena() override;
    // Inicializamos la cámara, fijamos FOV, orientaciones e invocamos view/project
    void iniCamera() override;
    // Se llama cada vez que cambie el tamaño de la ventana (w=ancho, h=alto)
    void resizeGL(int w, int h) override;
    // Calcula y envía la matriz de vista (view) al shader, tanto para cámara externa como interna
    void viewTransform() override;
    // Calcula y envía la matriz de proyección (proj) al shader, para ajustar FOV, zNear, zFar
    void projectTransform() override;
    // Se llama cada vez que toca redibujar: aquí pintamos Rick, puertas, cámara, suelo,...
    void paintGL() override;
    // Se llama cuando se mueve el ratón (coordenadas e)
    void mouseMoveEvent(QMouseEvent *e) override;
    // Se llama cuando se presiona una tecla (event contiene la tecla pulsada)
    void keyPressEvent(QKeyEvent* event) override;

private:
    // ----------------------
    // Funciones auxiliares
    // ----------------------

    // A partir de dos puntos mínimos y máximos de la caja envolvente de la escena,
    // calcula el centro y el radio de la escena para posicionar la cámara
    void calculaCentreRadiEscena(glm::vec3 puntMin, glm::vec3 puntMax);

    // Transformaciones específicas para cada objeto:

    // Ajusta la posición, rotación y escala del modelo de Rick antes de dibujarlo
    void rickTransform();
    // Ajusta la posición, rotación y escala del modelo de la videocámara antes de dibujarlo
    void videoCameraTransform();
    // Ajusta la posición y escala de una puerta (paralelepípedo)
    // Recibe pos: posición del centro de la base, mida: dimensiones en x,y,z
    void portaTransform(const glm::vec3 &pos, const glm::vec3 &mida);

    // ----------------------
    // Movimiento de la puerta roja (puerta3)
    // ----------------------

    // Determina si hay que abrir (true) o cerrar (false) la puerta roja
    void mourePorta(bool obrir);
    // En cada "tick" del temporizador, actualiza la posición de la puerta roja
    void actualitzarPorta();
    // Llamado cuando la puerta llega a destino para detener el movimiento
    void aturarMovimentPorta();

    // ----------------------
    // Movimiento automático de Rick
    // ----------------------

    void moureRick(); // Lógica de movimiento continuo de Rick (timer)

    // ----------------------
    // Variables miembro
    // ----------------------

    // Para las dos puertas azules: posiciones (centro de base) y tamaño común
    glm::vec3 posicionsPorta[2], tamanyPortas;
    // Para la videocámara: posición del centro de su base
    glm::vec3 posVideoCamera;
    // Ángulos esféricos de la cámara externa (phi: elevación, theta: azimut)
    float phi, theta;
    // Distancia desde la cámara externa hasta el centro de la escena
    float dist;

    // Controla si estamos usando la cámara externa (tercera persona) o interna (videocámara)
    bool cameraExterna;
    // FOV inicial de la cámara externa y FOV fija para la cámara interna
    float fovInicial, fovInterna = float(M_PI)/2.0f;

    // --------------
    // Movimiento automático de Rick
    // --------------
    // Temporizador Qt para que cada cierto intervalo se llame a moureRick()
    QTimer* temporitzadorRick;
    // Si true, Rick se moverá automáticamente; si false, solo movimiento manual
    bool movimentAutomatic = false;
    // Dirección actual de Rick (+1: hacia +X; -1: hacia -X)
    int direccioRick = +1;
    // Velocidad de Rick por "tick" (en unidades de escena)
    float velocitatRick = 0.25f;

    // --------------
    // Puerta roja (tercera puerta)
    // --------------
    glm::vec3 posicioPorta3, tamanyPorta3; // posición y tamaño de la puerta roja
    bool porta3Oberta = false;             // indica si está abierta o cerrada
    GLint porta3Loc;                       // ubicación (location) de la variable uniform "porta3" en el shader

    // Para controlar el deslizamiento de la puerta roja:
    QTimer* temporitzadorPorta;  // temporizador para animar la puerta roja
    bool portaEnMoviment = false; // true si la puerta está actualmente moviéndose
    glm::vec3 destiPorta, iniciPorta; // posiciones de destino e inicio del movimiento
    float velocitatPorta;        // velocidad de desplazamiento de la puerta roja

public slots:
    // Slots de Qt: funciones que se pueden conectar a señales, por ejemplo botones de interfaz

    // Cambia entre cámara externa e interna (slot conectado a un botón o acción de menú)
    void canviarCamera();
    // Resetea toda la escena a su estado inicial (slot conectado a botón o tecla R)
    void resetEscena();
    // Alterna movimiento automático de Rick (slot conectado a botón o tecla A)
    void alternarMovimentAutomatic();
};

#endif // MYGLWIDGET_H
