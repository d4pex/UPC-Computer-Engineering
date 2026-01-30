#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class ExamGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    ExamGLWidget (QWidget *parent=0);
    ~ExamGLWidget ();

  protected:
  
    int printOglError(const char file[], int line, const char func[]);
  
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
	virtual void modelTransformTower();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();

    void creaBuffersRick ();
    void creaBuffersTerra ();
    void creaBuffersTower ();
    void carregaShaders ();
    void calculaCapsaModel (Model &p, float &escala, float alcadaDesitjada, glm::vec3 &CentreBase);

    // VAO names
    GLuint VAO_Rick, VAO_Terra, VAO_Tower;
    // Program
    QOpenGLShaderProgram *program;
    // Viewport
    GLint ample, alt;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;

    // Matriu de posició i orientació
    glm::mat4 View;

    // models
    Model rick, tower;

    float alcadaTower=5;
  	float escalaTower;
	
	glm::vec3 posRick=glm::vec3(0,0,0);
	float alcadaRick=5;
	float escalaRick;
	float angleRick = 0;

    glm::vec3 centreEsc;
    float radiEsc, ra, fov, zn, zf;
	glm::vec3 obs, vrp, up, centreCaixaTower, centreCaixaRick;

    // Definim els paràmetres del material del cub
    glm::vec3 amb, diff, spec;
    float shin;

    glm::vec3 posFoc;
    float angleX, angleY;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
};

