#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"




class A3GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

    #define NUM_FOCUS 4

  public:
	A3GLWidget (QWidget *parent=0);
	~A3GLWidget ();

  protected:
    virtual void paintGL ( );

	// initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
	virtual void initializeGL ( );

	// resizeGL - És cridat quan canvia la mida del widget
	virtual void resizeGL (int width, int height);

	// mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event 
	// corresponent de ratolí
	virtual void mousePressEvent (QMouseEvent *event);
	virtual void mouseReleaseEvent (QMouseEvent *event);
	virtual void mouseMoveEvent (QMouseEvent *event);

  protected:
	//--------------------------------------------
	// Inicialitzacions
    virtual void iniEscena () = 0;
    virtual void carregaUniformLocations () = 0;
	void iniCamera ();
	//--------------------------------------------
	// Creació de buffers
	void creaBuffersModels(); 
    void creaBuffersQuadrat(glm::vec3 c, GLuint* VAO, float spec);
    //--------------------------------------------
    // Càrrega de shaders
    void carregaShaders();
	//--------------------------------------------
	void projectTransform ();
	void viewTransform ();
	//--------------------------------------------
	// Matrius de transformació de cada objecte
    void modelTransformSubmarine();
    void modelTransformSea();
    void modelTransformReef();

    void modelTransformDebug(glm::vec3 pos);


	//--------------------------------------------
	void calculaCapsaModel (Model &p, float &escala, float alcadaDesitjada, glm::vec3 &centreBase);



	//--------------------------------------------------------------
	//          GLOBALS D'ESCENA
	glm::vec3 centreEsc;
	float radiEsc, ra;
	GLint ample, alt;

	//--------------------------------------------------------------
	//          SHADERS
	//--------------------------------------------------------------
	QOpenGLShaderProgram *program;


	//--------------------------------------------------------------
	// model
	// enum models - els models estan en un array de VAOs (VAO_models), aquest enum és per fer més llegible el codi.
    typedef enum { REEF = 0, SUBMARINE = 1, SEA=2, NUM_MODELS = 3} ModelType;
	const char* modelPath[4]= {
                "./models/reef.obj",
                "./models/submarine.obj",
                "./models/sea.obj"};
	Model models[NUM_MODELS];
	// paràmetres calculats a partir de la capsa contenidora del model
	glm::vec3 centreBaseModels[NUM_MODELS];
	float escalaModels[NUM_MODELS];    
	//--------------------------------------------------------------
	// VAO names
	GLuint VAO_models[NUM_MODELS];
    GLuint VAO_Cub_Debug;

    //--------------------------------------------------------------
    // MATRIUS DE TRANSFORMACIÓ
    glm::mat4 Submarine_TG;


    //--------------------------------------------------------------
    // Posicions
    glm::vec3 SubmarinePos;
    glm::vec3 posFocus[NUM_FOCUS];
    glm::vec3 solPos;


	//--------------------------------------------------------------
	// INTERACCIÓ
	typedef  enum {NONE, ROTATE} InteractiveAction;
	InteractiveAction DoingInteractive;
	int xClick, yClick;
    float angleX, angleY;


    //--------------------------------------------------------------
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;


    glm::mat4 View;
};

