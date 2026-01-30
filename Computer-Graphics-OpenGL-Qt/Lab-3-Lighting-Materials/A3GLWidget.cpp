#include "A3GLWidget.h"
#include <glm/gtx/color_space.hpp>
#include <iostream>
#include <QTimer>
A3GLWidget::A3GLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  DoingInteractive = NONE;
}

A3GLWidget::~A3GLWidget ()
{
  if (program != NULL)
    delete program;
}

void A3GLWidget::initializeGL()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.0, 0.0, 0.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  carregaShaders();
  iniEscena ();
  iniCamera ();
}

void A3GLWidget::iniCamera ()
{
    ra = 1.0;
    angleY = -0.3;
    angleX = 0.4;
    projectTransform ();
    viewTransform ();
}




void A3GLWidget::paintGL ()
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //  glViewport (0, 0, ample, alt);


    projectTransform ();
    viewTransform ();
    // Esborrem el frame-buffer i el depth-buffer

    glClearColor(0/255.0f, 44/255.0f, 61/255.0f, 1.f);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //--------------------------------------------------------
    // Activem el VAO per a pintar el fons marí
    glBindVertexArray (VAO_models[REEF]);
    modelTransformReef();
    glDrawArrays(GL_TRIANGLES, 0, models[REEF].faces().size()*3);

    //--------------------------------------------------------
    // Activem el VAO per a pintar el Submarí
    glBindVertexArray (VAO_models[SUBMARINE]);
    modelTransformSubmarine();
    glDrawArrays(GL_TRIANGLES, 0, models[SUBMARINE].faces().size()*3);


    //--------------------------------------------------------

    glBindVertexArray(0);
}


void A3GLWidget::modelTransformSubmarine()
{
    glm::mat4 TG = glm::mat4(1.0f);   
    TG = glm::translate(TG, SubmarinePos);
    Submarine_TG = glm::scale(TG, glm::vec3(escalaModels[SUBMARINE]) );
    Submarine_TG = glm::rotate(Submarine_TG, glm::radians(-90.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &Submarine_TG[0][0]);
}

void A3GLWidget::modelTransformDebug(glm::vec3 pos)
{
    glm::mat4 TG = glm::mat4(1.0f);
    TG = glm::translate(TG,pos);
    TG = glm::scale(TG, glm::vec3(0.05, 0.05, 0.05));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void A3GLWidget::modelTransformSea()
{
    glm::mat4 TG = glm::mat4(1.0f);
    TG = glm::scale(TG, glm::vec3(escalaModels[REEF]) );
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void A3GLWidget::modelTransformReef()
{
    // Codi per a la TG necessària
    glm::mat4 TG = glm::mat4(1.0f);    
    TG = glm::scale(TG, glm::vec3(escalaModels[REEF]) );
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void A3GLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
  projectTransform ();
}


void A3GLWidget::projectTransform ()
{
  float fov, zn, zf;
  glm::mat4 Proj;  // Matriu de projecció
  
  fov = float(M_PI/3.0);
  zn = radiEsc*0.25;
  zf = 3*radiEsc;

  float rav =  width() /  (float)height() ;
  if(rav<ra){
      fov= 2*atan(tan(0.5*fov)/rav);
  }

  Proj = glm::perspective(fov, rav, zn, zf);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void A3GLWidget::viewTransform ()
{
  // Matriu de posició i orientació
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -1.3*radiEsc));
  View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc-glm::vec3(0,2,-2));

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}




void A3GLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void A3GLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void A3GLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / 180.0;    
    angleX += (e->y() - yClick) * M_PI / 180.0;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
} 


void A3GLWidget::calculaCapsaModel (Model &p, float &escala, float alcadaDesitjada, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = p.vertices()[0];
  miny = maxy = p.vertices()[1];
  minz = maxz = p.vertices()[2];
  for (unsigned int i = 3; i < p.vertices().size(); i+=3)
  {
    if (p.vertices()[i+0] < minx)
      minx = p.vertices()[i+0];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i+0];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }

  escala = alcadaDesitjada/(maxy-miny);
  centreBase[0] = (minx+maxx)/2.0; centreBase[1] = miny; centreBase[2] = (minz+maxz)/2.0;
}

void A3GLWidget::creaBuffersModels ()
{
	
	// Creació de buffers manuals (terra i mar)

	// Càrrega dels models
	for(int i=0;i<NUM_MODELS;i++) {
		models[i].load(modelPath[i]);	  
	}

	// Creació de VAOs i VBOs per pintar els models
	glGenVertexArrays(NUM_MODELS, &VAO_models[0]);

    float alcadaDesitjada[NUM_MODELS] = {5, 1, 15};

	for (int i = 0; i < NUM_MODELS; i++)
	{
		// Calculem la capsa contenidora del model
		calculaCapsaModel (models[i], escalaModels[i], alcadaDesitjada[i], centreBaseModels[i]);

		glBindVertexArray(VAO_models[i]);

		GLuint VBO[6];
		glGenBuffers(6, VBO);

		// geometria
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3, models[i].VBO_vertices(), GL_STATIC_DRAW);
		glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(vertexLoc);

		// Buffer de normals
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3, models[i].VBO_normals(), GL_STATIC_DRAW);
		glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(normalLoc);

		// Buffer de component ambient
        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3, models[i].VBO_matamb(), GL_STATIC_DRAW);
		glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(matambLoc);

		// Buffer de component difusa
        glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3, models[i].VBO_matdiff(), GL_STATIC_DRAW);
		glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(matdiffLoc);

		// Buffer de component especular
        glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3, models[i].VBO_matspec(), GL_STATIC_DRAW);
		glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(matspecLoc);

		// Buffer de component shininness
        glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3, models[i].VBO_matshin(), GL_STATIC_DRAW);
		glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(matshinLoc);

		glBindVertexArray(0);	  	  
	}
	glBindVertexArray (0);
}


void A3GLWidget::creaBuffersQuadrat(glm::vec3 c, GLuint* VAO, float spec)
{


    // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
             1.0f, 1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
             1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
             1.0f,-1.0f,-1.0f,
             1.0f, 1.0f,-1.0f,
             1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
             1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
             1.0f,-1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f,-1.0f,-1.0f,
             1.0f, 1.0f,-1.0f,
             1.0f,-1.0f,-1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f,-1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
             1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
             1.0f,-1.0f, 1.0f
        };
    static const GLfloat g_normal_buffer_data[] = {
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,//
             0.0f, 0.0f,-1.0f,
             0.0f, 0.0f,-1.0f,
             0.0f, 0.0f,-1.0f,//
             0.0f,-1.0f, 0.0f,
             0.0f,-1.0f, 0.0f,
             0.0f,-1.0f, 0.0f,//
             0.0f, 0.0f,-1.0f,
             0.0f, 0.0f,-1.0f,
             0.0f, 0.0f,-1.0f,//
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,//
             0.0f,-1.0f, 0.0f,
             0.0f,-1.0f, 0.0f,
             0.0f,-1.0f, 0.0f,//
             0.0f, 0.0f, 1.0f,
             0.0f, 0.0f, 1.0f,
             0.0f, 0.0f, 1.0f,//
             1.0f, 0.0f, 0.0f,
             1.0f, 0.0f, 0.0f,
             1.0f, 0.0f, 0.0f,//
             1.0f, 0.0f, 0.0f,
             1.0f, 0.0f, 0.0f,
             1.0f, 0.0f, 0.0f,//
             0.0f, 1.0f, 0.0f,
             0.0f, 1.0f, 0.0f,
             0.0f, 1.0f, 0.0f,//
             0.0f, 1.0f, 0.0f,
             0.0f, 1.0f, 0.0f,
             0.0f, 1.0f, 0.0f,//
             0.0f, 0.0f, 1.0f,
             0.0f, 0.0f, 1.0f,
             0.0f, 0.0f, 1.0f//
        };

    const int vertices = 36;

    glm::vec3 col[vertices];
    glm::vec3 spc[vertices];
    glm::vec3 sp = glm::vec3(spec, spec, spec);
    GLfloat shs[vertices];
    GLfloat sh = 100;
    for(int i=0;i<vertices;i++) {
        col[i] = c;
        spc[i] =sp;
        shs[i] = sh;
    }




  // VAO
  glGenVertexArrays(1, VAO);
  glBindVertexArray(*VAO);

  GLuint VBO[6];
  glGenBuffers(6, VBO);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_normal_buffer_data), g_normal_buffer_data, GL_STATIC_DRAW);
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);
  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);
  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);

  if(spec>0){
    glBufferData(GL_ARRAY_BUFFER, sizeof(spc), col, GL_STATIC_DRAW);
  } else {
    glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);
  }
  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(shs), shs, GL_STATIC_DRAW);
  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray (0);
}




void A3GLWidget::carregaShaders()
{
    // Creem els shaders per al fragment shader i el vertex shader
    QOpenGLShader fs (QOpenGLShader::Fragment, this);
    QOpenGLShader vs (QOpenGLShader::Vertex, this);
    // Carreguem el codi dels fitxers i els compilem
    fs.compileSourceFile("./shaders/basicLlumShader.frag");
    vs.compileSourceFile("./shaders/basicLlumShader.vert");
    // Creem el program
    program = new QOpenGLShaderProgram(this);
    // Li afegim els shaders corresponents
    program->addShader(&fs);
    program->addShader(&vs);
    // Linkem el program
    program->link();
    // Indiquem que aquest és el program que volem usar
    program->bind();

    // Obtenim identificador per a l'atribut “vertex” del vertex shader
    vertexLoc = glGetAttribLocation (program->programId(), "vertex");
    // Obtenim identificador per a l'atribut “normal” del vertex shader
    normalLoc = glGetAttribLocation (program->programId(), "normal");
    // Obtenim identificador per a l'atribut “matamb” del vertex shader
    matambLoc = glGetAttribLocation (program->programId(), "matamb");
    // Obtenim identificador per a l'atribut “matdiff” del vertex shader
    matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
    // Obtenim identificador per a l'atribut “matspec” del vertex shader
    matspecLoc = glGetAttribLocation (program->programId(), "matspec");
    // Obtenim identificador per a l'atribut “matshin” del vertex shader
    matshinLoc = glGetAttribLocation (program->programId(), "matshin");

    // Demanem identificadors per als uniforms del vertex shader
    transLoc = glGetUniformLocation (program->programId(), "TG");
    projLoc = glGetUniformLocation (program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");

    carregaUniformLocations();

}


