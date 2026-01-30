#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

public:
    // Constructor y destructor
    // QWidget *parent=0 indica que, por defecto, este widget no tiene padre
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();

protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void modelTransformTerra ();
    virtual void modelTransformRick ();
    virtual void modelTransformTower1();
    virtual void modelTransformTower2();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();

    void creaBuffersRick ();
    void creaBuffersTerra ();
    void creaBuffersTower ();
    void carregaShaders ();
    void calculaCapsaModel (Model &p, float &escala, float alcadaDesitjada, glm::vec3 &CentreBase);

private:
    // ----------------------
    // Funciones auxiliares
    // ----------------------

    // A partir de dos puntos mínimos y máximos de la caja envolvente de la escena,
    // calcula el centro y el radio de la escena para posicionar la cámara
    void calculaCentreRadiEscena(glm::vec3 puntMin, glm::vec3 puntMax);


    float phi, theta;
    // Distancia desde la cámara externa hasta el centro de la escena
    float dist;
    // FOV inicial de la cámara externa y FOV fija para la cámara interna
    float fovInicial, fovZoom = 0;

    bool cameraExterna = true;

    // per fer el phong pasant la posicio de la camera
    GLuint locPosicioFocus;
    glm::vec3 posicioFocus;

public slots:
    // Cambia entre cámara externa e interna (slot conectado a un botón o acción de menú)
    void canviarCamera();
    void zoomIN();
    void zoomOUT();
    void resetEscena();

};
