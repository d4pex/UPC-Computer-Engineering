// Incluimos las funcionalidades básicas de OpenGL 3.3 Core a través de Qt
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>             // Permite crear un widget con contexto OpenGL
#include <QOpenGLShader>             // Para manejar shaders individuales en Qt
#include <QOpenGLShaderProgram>      // Para compilar y vincular programas de shaders
#include <QKeyEvent>                 // Para capturar eventos de teclado en el widget
#include "glm/glm.hpp"               // Biblioteca GLM para vectores y matrices (math en OpenGL)

#ifndef __MyGLWidget__
#define __MyGLWidget__

// Declaración de la clase MyGLWidget, que extiende QOpenGLWidget y utiliza funciones OpenGL 3.3
class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT  // Macro de Qt que habilita señales y slots en la clase

public:
    // Constructor. Puede recibir un puntero al widget padre (por defecto nullptr).
    MyGLWidget (QWidget *parent=0);
    // Destructor. Se encargará de liberar recursos (por ejemplo, el QOpenGLShaderProgram).
    ~MyGLWidget ();

    // Resetea el tiempo interno utilizado en modo automático
    void resetTime();

    // Devuelve el valor actual de la variable de tiempo 't'
    float getTime();

    // Función inline que devuelve si actualmente estamos en modo manual (true) o automático (false)
    bool esModeManual(){
        return modeManual;
    }

private:
    /// ----------------------------------------------------------------
    /// Variables de estado internas
    /// ----------------------------------------------------------------

    // Tiempo acumulado para animaciones automáticas (no tocar en modo manual).
    float t = 0;

    // Indica si el modo es manual (true) o automático (false).
    bool modeManual = true;

public slots:
    // Slot que se llama periódicamente para incrementar 't' y forzar una nueva llamada a paintGL()
    void tick();
    // Slot para cambiar el modo de control (manual/automático)
    void setModeManual(bool esManual){
        modeManual = esManual;
    }

signals:
    // Señal que se emite al actualizar el tiempo (se puede conectar a un display de tiempo en la interfaz)
    void timeUpdate(double time);

protected:
    // Método de inicialización del contexto OpenGL: aquí cargamos shaders y creamos buffers.
    virtual void initializeGL ();

    // Método que se llama cada vez que es necesario repintar la ventana. Aquí va todo el código de dibujo.
    virtual void paintGL ();

    // Método que se llama cuando cambia el tamaño del widget; se ajustan las variables de viewport si hace falta.
    virtual void resizeGL (int width, int height);

    // Método que se llama cuando el usuario presiona una tecla; gestionamos controles de los péndulos en modo manual.
    virtual void keyPressEvent (QKeyEvent *event);

private:
    // Variables de rotación (con ejemplo para 2 objetos, aunque luego se usan ángulos concretos de péndulo)
    float rotation1 = 0, rotation2 = 0;

    // Constante que define cuántos vértices usaremos para aproximar círculos (bolas de los péndulos).
    const int CIRCLE_SAMPLES = 40;

    // Array con los ángulos individuales de los cuatro péndulos. En modo automático se van actualizando.
    float angles[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    // Ángulos específicos de los péndulos 1 y 4 en modo manual.
    float angleP1 = 0.0f, angleP4 = 0.0f;

    // Variable para indicar si acabamos de cambiar a modo automático (para inicializar ángulos).
    bool canviAutomatic = false;

    // Fase actual en el ciclo automático: 1 = mover péndulo 1, 2 = mover péndulo 4.
    int faseAuto = 1;

    //-------------------------------------------------
    // Funciones auxiliares de dibujo: cada una pinta un objeto concreto.
    // Estas rutinas encapsulan las llamadas OpenGL necesarias para cada parte de la escena.
    void pintaTerra();                     // Dibuja la “base / suelo” con efecto de bandas de color.
    void pintaSuport(float d, float angle);// Dibuja la barra (soporte) de cada péndulo.
    void pintaBola(float d, float angle);  // Dibuja la bola final de cada péndulo.
    void pintaEstrella();                  // Dibuja la estrella decorativa (adjunta al soporte).

    //-------------------------------------------------
    // Métodos para crear VBOs y VAOs (buffers de vértices y colores) antes de dibujar:
    void creaBuffers();          // Llama a las cuatro funciones de creación de buffers.
    void creaBufferBola();       // Crea VBOs/VAO para las bolas (circulos).
    void creaBufferTerra();      // Crea VBOs/VAO para la base/“suelo” con tiras de color.
    void creaBufferSuport();     // Crea VBOs/VAO para el soporte (barra vertical) de los péndulos.
    void creaBufferEstrella();   // Crea VBOs/VAO para la estrella (polígono estrella).

    //-------------------------------------------------
    // Funciones de transformación matricial (model matrix) para cada objeto:
    void transformacioBola(float d, float angle);   // Calcula y envía la matriz de la bola
    void transformacioSuport(float d, float angle); // Calcula y envía la matriz del soporte
    void transformacioTerra();                       // Usa la matriz identidad, solo base sin transformaciones

    //-------------------------------------------------
    // Compilación y enlace de shaders (vertex + fragment) y obtención de ubicaciones.
    void carregaShaders ();

    // Función auxiliar para generar buffers de vértices/colores: recibe arrays de glm::vec3 y crea VBOs.
    void createVBOs(int size, glm::vec3 Colors[], glm::vec3 Vertices[]);

    //-------------------------------------------------
    // Localizaciones de atributos y uniformes en el shader:
    GLuint vertexLoc;   // Índice del atributo “vertex” en el vertex shader
    GLuint colorLoc;    // Índice del atributo “color” en el vertex shader
    GLuint transLoc;    // Ubicación de la uniform “TG” (matriz de transformación)

    // El programa de shaders compilado (vertex + fragment)
    QOpenGLShaderProgram *program;

    // IDs de VAO para cada objeto: base, soporte, bola y estrella
    GLuint VAO_TERRA, VAO_SUPORT, VAO_BOLA, VAO_ESTRELLA;

    // Variables para guardar el tamaño de la ventana (en píxeles) – se usan en retina/Mac en paintGL
    GLint ample, alt;

    // Colores definidos como glm::vec3 (RGB) para usar en buffers y en el shader
    glm::vec3 COLOR_BLANC       = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 COLOR_VERMELL     = glm::vec3(1.0f, 0.5f, 0.3f);
    glm::vec3 COLOR_BLAU        = glm::vec3(48.0f/255.0f, 58.0f/255.0f, 158.0f/255.0f);
    glm::vec3 COLOR_BLAU_FLUIX  = glm::vec3(109.0f/255.0f, 117.0f/255.0f, 201.0f/255.0f);
    glm::vec3 COLOR_GROC        = glm::vec3(239.0f/255.0f, 251.0f/255.0f, 38.0f/255.0f);
    glm::vec3 COLOR_MARRO       = glm::vec3(153.0f/255.0f, 121.0f/255.0f, 52.0f/255.0f);

};

#endif
