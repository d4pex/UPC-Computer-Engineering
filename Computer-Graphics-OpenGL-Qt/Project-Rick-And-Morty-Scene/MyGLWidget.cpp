// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>


// ============================================================
// Recordatori sintaxi:
//    glm::mat4 Proj = glm::ortho (l, r, b, t, zn, zf);
// ============================================================


// Constructor de MyGLWidget
MyGLWidget::MyGLWidget(QWidget* parent) : ExamGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);

}

// Destructor de MyGLWidget
MyGLWidget::~MyGLWidget() {
    // Llamamos explícitamente al destructor de la clase base para limpiar recursos, aunque Qt lo haría automáticamente.
    ExamGLWidget::~ExamGLWidget();
}

void MyGLWidget::initializeGL ()
{
    // Cal inicialitzar l'ús de les funcions d'OpenGL
    initializeOpenGLFunctions();

    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    glEnable(GL_DEPTH_TEST);
    carregaShaders();
    iniEscena ();
    iniCamera ();
}

void MyGLWidget::iniEscena ()
{
    creaBuffersRick();
    creaBuffersTower();
    creaBuffersTerra();

    glm::vec3 puntMin(-5.0f, 0.0f, -10.0f);
    glm::vec3 puntMax( 5.0f, 5.0f,  10.0f);

    // Calculamos el centro y el radio de la escena basándonos en esas cotas
    calculaCentreRadiEscena(puntMin, puntMax);
}

void MyGLWidget::iniCamera ()
{
    dist = radiEsc * 2.0f;

    fovInicial = 2.0f * asin(radiEsc / dist);

    fov = fovInicial;

    zn = dist - radiEsc;
    zf  = dist + radiEsc;

    phi   = M_PI / 4.0f;
    theta = 0.0f;


    // Enviamos la vista y proyección iniciales
    projectTransform();
    viewTransform();
}

void MyGLWidget::paintGL ()
{
    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //  glViewport (0, 0, ample, alt);

    // Esborrem el frame-buffer i el depth-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Pintem el terra
    glBindVertexArray (VAO_Terra);
    modelTransformTerra ();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Pintem el Rick
    glBindVertexArray (VAO_Rick);
    modelTransformRick ();
    glDrawArrays(GL_TRIANGLES, 0, rick.faces().size()*3);

    // Pintem el Tower 1
    glBindVertexArray (VAO_Tower);
    modelTransformTower1 ();
    glDrawArrays(GL_TRIANGLES, 0, tower.faces().size()*3);

    // Pintem el Tower 2
    glBindVertexArray (VAO_Tower);
    modelTransformTower2 ();
    glDrawArrays(GL_TRIANGLES, 0, tower.faces().size()*3);

    glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
    ra = float(w) / float(h);

    if (cameraExterna) {
        // Si la cámara externa está activa:
        // Si ra < 1 (ventana más alta que ancha), aumentamos FOV vertical para no recortar
        if (ra < 1.0f) {
            // fov nuevo = 2 * atan( tan(fovInicial/2) / ra )
            // Ajusta fov para que la escena no se recorte en pantallas altas
            fov = 2.0f * atan(tan(0.5f * (fovInicial + fovZoom)) / ra);
        } else {
            // Si ra >= 1, seguimos con fovInicial
            fov = fovInicial + fovZoom;
        }
    }

    // Reenviamos las transformaciones actualizadas
    viewTransform();
    projectTransform();

    // Indicamos a OpenGL que el viewport abarca toda la ventana (desde 0,0 hasta w,h)
    glViewport(0, 0, w, h);

}

void MyGLWidget::modelTransformRick ()
{
    glm::mat4 TG(1.f);
    TG = glm::rotate(TG, float(M_PI)/2.0f, glm::vec3(0, 1, 0));
    TG = glm::scale(TG, glm::vec3 (escalaRick, escalaRick, escalaRick));
    TG = glm::translate(TG, -centreCaixaRick);
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTower1()
{
    glm::vec3 posTorre=glm::vec3(-3,0,5);
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, posTorre);
    TG = glm::scale(TG, glm::vec3 (escalaTower, escalaTower, escalaTower));
    TG = glm::translate(TG, -centreCaixaTower);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTower2()
{
    glm::vec3 posTorre=glm::vec3(-3,0,-5);
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, posTorre);
    TG = glm::scale(TG, glm::vec3 (escalaTower, escalaTower, escalaTower));
    TG = glm::translate(TG, -centreCaixaTower);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
    glm::mat4 TG(1.f);
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
    glm::mat4 Proj;

    if (cameraExterna) {
        Proj = glm::perspective(fov, ra, zn, zf);
    } else {
        Proj = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 25.0f);
    }

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);

}

void MyGLWidget::viewTransform ()
{
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
        View = glm::translate(View, -centreEsc);
    } else {
        glm::vec3 obs = glm::vec3(0,20,0);
        glm::vec3 vrp = glm::vec3(0,0,0);
        glm::vec3 up  = glm::vec3(1, 0, 0);
        View = glm::lookAt(obs, vrp, up);
    }

    // Enviamos la matriz View al shader en la variable uniform "View" (ubicación viewLoc)
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);

    //Pasem posicio de la camera
    glm::vec3 posicioFocus = glm::vec3(0.0f, 0.0f, 0.0f);
    glUniform3fv(locPosicioFocus, 1, &posicioFocus.x);

}

// calculaCentreRadiEscena(): a partir de puntMin y puntMax calcula centroEscena y radiEscena
void MyGLWidget::calculaCentreRadiEscena(glm::vec3 puntMin, glm::vec3 puntMax) {
    // Centro = punto medio entre puntMin y puntMax
    centreEsc = (puntMin + puntMax) / 2.0f;

    // half = vector desde el centro hasta puntMax
    glm::vec3 half = puntMax - centreEsc;
    // radio = distancia euclídea desde el centro hasta ese punto máximo
    radiEsc = sqrt(half.x * half.x + half.y * half.y + half.z * half.z);
}

// canviarCamera(): slot que también alterna cámara externa/interna (similar a pulsar C)
void MyGLWidget::canviarCamera() {
    makeCurrent();
    cameraExterna = !cameraExterna;
    viewTransform();
    resizeGL(width(), height());
    update();
}

void MyGLWidget::zoomIN() {
    makeCurrent();
    if(cameraExterna and fov > 0.1f) { // limit angle aproximat
        fov -= 0.1f;
        fovZoom -= 0.1f;
        projectTransform();
    }
    update();
}

void MyGLWidget::zoomOUT() {
    makeCurrent();
    if(cameraExterna and fov < 3.0f) { // limit angle aproximat
        fov += 0.1f;
        fovZoom += 0.1f;
        projectTransform();
    }
    update();
}

void MyGLWidget::resetEscena() {
    cameraExterna = true;
    iniCamera();
    resizeGL(width(), height());
    update();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_C:
            // Tecla C: alternar entre cámara externa e interna
            cameraExterna = !cameraExterna;
            // Una vez cambiada la cámara, actualizamos matrices y viewport
            viewTransform();
            projectTransform();
            resizeGL(width(), height());
            update();
            break;

        case Qt::Key_A:
            if(cameraExterna and fov > 0.1f) { // limit angle aproximat
                fov -= 0.1f;
                fovZoom -= 0.1f;
                projectTransform();
            }
            break;

        case Qt::Key_Z:
            if(cameraExterna and fov < 3.0f) { // limit angle aproximat
                fov += 0.1f;
                fovZoom += 0.1f;
                projectTransform();
            }
            break;

        case Qt::Key_R:
            // Tecla R: resetear la escena a su estado inicial
            resetEscena();
            break;

        default:
            // Para cualquier otra tecla, delegamos a la implementación base (por ejemplo, Quitar clipping planes, etc.)
            event->ignore();
            break;
    }
    update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
    xClick = e->x();
    yClick = e->y();

    if (e->button() & Qt::LeftButton &&
        ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
    {
        DoingInteractive = ROTATE;
    }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
    DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();
    makeCurrent(); // Contexto activo

    // Solo rotamos si estamos en cámara externa y el modo interactivo es ROTATE
    if (cameraExterna && DoingInteractive == ROTATE) {
        theta -= (e->x() - xClick) * 0.005f;  // moviendo a la derecha, theta aumenta => giramos escena a la derecha
        phi   += (e->y() - yClick) * 0.005f;  // moviendo hacia arriba, phi aumenta => giramos escena hacia abajo

        // Actualizamos la vista con los nuevos phi y theta
        viewTransform();
    }
    xClick = e->x();
    yClick = e->y();

    update();
}

void MyGLWidget::calculaCapsaModel (Model &p, float &escala, float alcadaDesitjada, glm::vec3 &centreBase)
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

void MyGLWidget::carregaShaders()
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

    // Demanem identificadors per als uniforms dels shaders
    transLoc = glGetUniformLocation (program->programId(), "TG");
    projLoc = glGetUniformLocation (program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");

    locPosicioFocus = glGetUniformLocation(program->programId(), "posFoc");
}

void MyGLWidget::creaBuffersRick ()
{
    // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
    rick.load("./models/Rick.obj");

    // Calculem la capsa contenidora del model
    calculaCapsaModel (rick, escalaRick, 1.7f, centreCaixaRick);

    // Creació del Vertex Array Object del Rick
    glGenVertexArrays(1, &VAO_Rick);
    glBindVertexArray(VAO_Rick);

    // Creació dels buffers del model rick
    GLuint VBO_Patr[6];
    // Buffer de posicions
    glGenBuffers(6, VBO_Patr);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*rick.faces().size()*3*3, rick.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Buffer de normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*rick.faces().size()*3*3, rick.VBO_normals(), GL_STATIC_DRAW);

    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);

    // Buffer de component ambient
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*rick.faces().size()*3*3, rick.VBO_matamb(), GL_STATIC_DRAW);

    glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matambLoc);

    // Buffer de component difusa
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*rick.faces().size()*3*3, rick.VBO_matdiff(), GL_STATIC_DRAW);

    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    // Buffer de component especular
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*rick.faces().size()*3*3, rick.VBO_matspec(), GL_STATIC_DRAW);

    glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matspecLoc);

    // Buffer de component shininness
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*rick.faces().size()*3, rick.VBO_matshin(), GL_STATIC_DRAW);

    glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matshinLoc);

    glBindVertexArray(0);
}

void MyGLWidget::creaBuffersTower ()
{
    // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
    tower.load("./models/WatchTower.obj");

    // Calculem la capsa contenidora del model
    calculaCapsaModel (tower, escalaTower, 5.0f, centreCaixaTower);

    // Creació del Vertex Array Object del tower
    glGenVertexArrays(1, &VAO_Tower);
    glBindVertexArray(VAO_Tower);

    // Creació dels buffers del model tower
    GLuint VBO_Patr[6];
    // Buffer de posicions
    glGenBuffers(6, VBO_Patr);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*tower.faces().size()*3*3, tower.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Buffer de normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*tower.faces().size()*3*3, tower.VBO_normals(), GL_STATIC_DRAW);

    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);

    // Buffer de component ambient
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*tower.faces().size()*3*3, tower.VBO_matamb(), GL_STATIC_DRAW);

    glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matambLoc);

    // Buffer de component difusa
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*tower.faces().size()*3*3, tower.VBO_matdiff(), GL_STATIC_DRAW);

    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    // Buffer de component especular
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*tower.faces().size()*3*3, tower.VBO_matspec(), GL_STATIC_DRAW);

    glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matspecLoc);

    // Buffer de component shininness
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*tower.faces().size()*3, tower.VBO_matshin(), GL_STATIC_DRAW);

    glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matshinLoc);

    glBindVertexArray(0);
}

void MyGLWidget::creaBuffersTerra ()
{
    // VBO amb la posició dels vèrtexs
    glm::vec3 posterra[4] = {
        glm::vec3(-5.0, 0.0, -10.0),
        glm::vec3(-5.0, 0.0,  10.0),
        glm::vec3( 5.0, 0.0, -10.0),
        glm::vec3( 5.0, 0.0,  10.0)
    };

    // VBO amb la normal de cada vèrtex
    glm::vec3 norm (0,1,0);
    glm::vec3 normterra[6] = {
        norm, norm, norm, norm
    };

    // Definim el material del terra
    glm::vec3 amb(0.2,0.2,0.2);
    glm::vec3 diff1(0.77, 0.60, 0.48);
    glm::vec3 diff2(0, 0, 1);
    glm::vec3 spec(0,0,0);
    float shin = 10;

    // Fem que aquest material afecti a tots els vèrtexs per igual
    glm::vec3 matamb[] = {
        amb, amb, amb, amb
    };
    glm::vec3 matdiff[6] = {
        diff1, diff1, diff2, diff2
    };
    glm::vec3 matspec[6] = {
        spec, spec, spec, spec
    };
    float matshin[6] = {
        shin, shin, shin, shin
    };

    // Creació del Vertex Array Object del terra
    glGenVertexArrays(1, &VAO_Terra);
    glBindVertexArray(VAO_Terra);

    GLuint VBO_Terra[6];
    glGenBuffers(6, VBO_Terra);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

    // Activem l'atribut normalLoc
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);

    // Buffer de component ambient
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(matamb), matamb, GL_STATIC_DRAW);

    glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matambLoc);

    // Buffer de component difusa
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(matdiff), matdiff, GL_STATIC_DRAW);

    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    // Buffer de component especular
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(matspec), matspec, GL_STATIC_DRAW);

    glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matspecLoc);

    // Buffer de component shininness
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(matshin), matshin, GL_STATIC_DRAW);

    glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matshinLoc);

    glBindVertexArray(0);
}


