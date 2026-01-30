// Forzamos a GLM a usar radianes en lugar de grados en sus funciones de transformación.
// (Si no se define, algunas funciones de GLM podrían interpretar ángulos en grados.)
#define GLM_FORCE_RADIANS

// Incluimos las cabeceras necesarias de Qt para trabajar con OpenGL
#include <QOpenGLFunctions_3_3_Core>   // Funciones de OpenGL 3.3 (core profile)
#include <QOpenGLWidget>               // Widget de Qt que nos permite renderizar con OpenGL
#include <QOpenGLShader>               // Clase base para compilar shaders
#include <QOpenGLShaderProgram>        // Clase para gestionar un programa de shaders (vértices + fragment)
// Eventos de teclado y ratón
#include <QKeyEvent>
#include <QMouseEvent>

// Incluimos GLM para operaciones con vectores y matrices
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Model.h (contiene la clase/modelo que carga los .obj y almacena VAOs/VBOs)
// A3GLWidget.h (clase base proporcionada en el esqueleto de la actividad)
#include "model.h"
#include "A3GLWidget.h"

// Declaramos la clase MyGLWidget heredando de A3GLWidget
class MyGLWidget : public A3GLWidget
{
    Q_OBJECT  // Macro necesaria para que Qt reconozca las señales/slots

public:
    // Constructor y destructor
    MyGLWidget (QWidget *parent = 0);
    ~MyGLWidget ();

protected:
    // Métodos virtuales que sobreescribimos de A3GLWidget:
    void initializeGL() override;          // Inicialización de OpenGL (al arrancar la aplicación)
    void iniEscena () override;            // Inicialización de la escena (cargar modelos, centros, radios, focos, etc.)
    void paintGL() override;               // Función principal de renderizado (se llama cada vez que hay que pintar la ventana)
    void carregaUniformLocations () override;  // Obtener ubicaciones de variables uniform en el pipeline de shaders
    void keyPressEvent (QKeyEvent *event) override; // Control de teclado para mover el submarino

    // ----------------------
    // UNIFORMS PARA ILUMINACIÓN PRINCIPAL
    // ----------------------

    GLuint colAmbLoc;     // Ubicación en el shader de la variable 'colorAmbient'
    GLuint colLlumLoc;    // Ubicación en el shader de la variable 'colorLlum' (color de la luz puntual global)
    GLuint posLlumLoc;    // Ubicación de 'posLlum' (posición de la luz global)
    GLuint posCameraLoc;  // Ubicación de 'posCamera' (posición de la cámara, para cálculo especular)
    float intensitatFoc = 0.5f;  // Intensidad (factor escalar 0..1) para multiplicar el color de la luz principal

    // ----------------------
    // FOCOS DEL SUBMARINO (4 focos frontales)
    // ----------------------

    // Posiciones fijas de los focos en el sistema de coordenadas del modelo del submarino (SCM)
    // Estas coordenadas vienen dadas en el enunciado:
    //   ( 0.44, 0.85, 3.47 ),
    //   ( 0.88, 0.85, 2.83 ),
    //   (-0.95, 0.85, 2.83 ),
    //   (-0.45, 0.85, 3.47 )
    std::vector<glm::vec3> posFocusSubModel;

    // Uniforms de los 4 focos del submarino (arrays de 4)
    GLuint posFocusSubLoc[4];    // posición de cada foco en coordenadas de mundo
    GLuint dirFocusSubLoc[4];    // dirección del cono de cada foco (normalizada)
    GLuint colFocusSubLoc[4];    // color de cada foco
    GLuint conectarFocusSub[4];  // bool para encender/apagar cada foco

    // Color de cada foco (vector de glm::vec3), inicialmente blanco (1.0,1.0,1.0)
    std::vector<glm::vec3> colFocusSubVec = {
        glm::vec3(1.0f), glm::vec3(1.0f),
        glm::vec3(1.0f), glm::vec3(1.0f)
    };

    // Estado de encendido de cada foco (true = encendido, false = apagado)
    std::vector<bool> focusConectat = { true, true, true, true };

    // ----------------------
    // CONTROL DE FASES DE DIBUJADO
    // ----------------------
    // faseDibuixat: nos permitirá separar objetos sólidos (fase = 0) de objetos translúcidos (fase = 1)
    // pintarMar: flag para avisar al fragment shader de que está pintando la cara del mar (prisma)
    GLuint faseDibuixatLoc;
    GLuint pintarMarLoc;

    // ----------------------
    // MATRIZ COMBINADA PROYECCIÓN-VISTA
    // ----------------------
    // Necesaria en el fragment shader para calcular posiciones en NDC (cuando dibujamos el rastro
    // de los focos sobre el mar o para el fragment de mar con ruido).
    GLuint projViewLoc;

public slots:
    // Slot para cambiar la intensidad del foco global (slider en la interfaz)
    void setIntensitatFoc(int valor);

    // Slots para cada uno de los focos del submarino:
    // controlFocusN: enciende/apaga el foco N
    // botoColorN: abre un diálogo para escoger el color del foco N
    // N = 0,1,2,3
    void controlFocus0();
    void botoColor0();

    void controlFocus1();
    void botoColor1();

    void controlFocus2();
    void botoColor2();

    void controlFocus3();
    void botoColor3();
};
