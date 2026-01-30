// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

// Macros per ajudar a imprimir errors d'OpenGL
#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

// Funció interna per capturar errors d'OpenGL
int MyGLWidget::printOglError(const char fitxer[], int linia, const char funcio[])
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = nullptr;
    switch (glErr)
    {
    case 0x0500:
        error = "GL_INVALID_ENUM";
        break;
    case 0x501:
        error = "GL_INVALID_VALUE";
        break;
    case 0x502:
        error = "GL_INVALID_OPERATION";
        break;
    case 0x503:
        error = "GL_STACK_OVERFLOW";
        break;
    case 0x504:
        error = "GL_STACK_UNDERFLOW";
        break;
    case 0x505:
        error = "GL_OUT_OF_MEMORY";
        break;
    default:
        error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in fitxer %s @ linia %d: %s funció: %s\n",
               fitxer, linia, error, funcio);
        retCode = 1;
    }
    return retCode;
}

// Constructor: inicialitza aspectes de l'escena i crea monedes aleatòries
MyGLWidget::MyGLWidget(QWidget *pare) : ExamGLWidget(pare) {
    // Connectem el timer per girar les monedes al slot girarMonedes()
    connect(&timerCoins, SIGNAL(timeout()), this, SLOT(girarMonedes()));
    timerCoins.start(80);           // Cada 80 ms, timeout -> giraMonedes()
    srand(time(NULL));              // Inicialitzem la llavor per a rand()

    // Omplim el vector de posicionsDisponibles amb totes les caselles de terra (laberint[i][j] == 0)
    for (int i = 0; i < numFiles; i++) {
        for (int j = 0; j < numColumnes; ++j) {
            if (laberint[i][j] == 0) {
                // Emmagatzemem la posició a nivell món: x = j, y = 0, z = i
                posicionsDisponibles.emplace_back(glm::vec3(j, 0, i), false);
            }
        }
    }

    // Generem aleatòriament 10 monedes en posicions de terra sense solapaments
    int numMaxMonedes = 10;
    int compt = 0;
    while (compt < numMaxMonedes) {
        int indexAleatori = std::rand() % posicionsDisponibles.size();
        // Si aquella posició encara no té moneda (bool == false)
        if (posicionsDisponibles[indexAleatori].second == false) {
            posicionsMonedesFinals.push_back(posicionsDisponibles[indexAleatori].first);
            posicionsDisponibles[indexAleatori].second = true;
            ++compt;
        }
    }

    // Angles inicials de rotació amb el ratolí (per tercera persona)
    angleX = angleY = M_PI / 4.0f;
    focusEscena = true;        // Focus inicial = càmera (no Morty)
    primeraPersona = false;     // Càmera inicial = tercera persona
}

// Destructora buida (no cal alliberar res addicional aquí)
MyGLWidget::~MyGLWidget() {
}

// Inicialització de l'escena: posicions i orientacions de Morty i fantasma
void MyGLWidget::iniEscena()
{
    // Morty:
    angleMorty = 0.0f;                            // Morty mira cap a Z+
    posicioMorty = glm::vec3(1.5f, 0.0f, 1.5f);   // Centre de la base a (1.5, 0, 1.5)
    posicioMortyLab = glm::vec2(1, 1);            // Casella (fila=1, columna=1)
    direccioMorty = glm::vec3(0.0f, 0.0f, 1.0f);   // Vector unitari cap a Z+

    // Fantasma:
    angleFantasma = 0.0f;                         // Inicialment alineat
    posicioFantasma = glm::vec3(8.5f, 0.0f, 10.5f);// Casella (fila=9, columna=7)
    posicioFantasmaLab = glm::vec2(9, 7);         // Casella (9,7)
    direccioFantasma = glm::vec3(-1.0f, 0.0f, 0.0f);// Inicial direcció cap a X- (es calcularà amb update)

    // Creem la capsa contenidora de tota l'escena (dimensionada per tot el laberint)
    creaCapsaContenidora(glm::vec3(15.0f, 1.5f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

// Inicialització de la càmera en tercera persona
void MyGLWidget::iniCamera()
{
    // Angles inicials d'Euler (psi, theta) = (π/4, π/4)
    angleY = angleX = M_PI / 4.0f;

    // Distància d’observador respecte al centre de l'escena
    float d = 2.0f * radiEsc;

    // Punt visual (VRP) = centre de l'escena
    puntVisualitzacio = centreEsc;
    // Observador situat darrere (+Z) a distància d
    observador = puntVisualitzacio + (d * glm::vec3(0.0f, 0.0f, 1.0f));

    // Vector amunt per lookAt
    vectorAmunt = glm::vec3(0.0f, 1.0f, 0.0f);

    // Plans de tall (znear i zfar)
    zn = d - radiEsc;
    zf = d + radiEsc;

    // FOV calculat a partir de radi/d
    float alpha_v = asinf(radiEsc / d);
    fov = 2.0f * alpha_v;

    ra = 1.0f; // Relació d'aspecte temporal; es modifica a resizeGL()

    projectTransform();
    viewTransform();
}

// Pintat de tota la finestra OpenGL: Morty, monedes, fantasma i laberint
void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --------- PINTEM A MORTY -------------
    glBindVertexArray(VAO_models[MORTY]);
    modelTransformMorty();
    glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size() * 3);

    // --------- PINTEM LES MONEDES GIRATÒRIES -------------
    for (unsigned int i = 0; i < posicionsMonedesFinals.size(); ++i) {
        glBindVertexArray(VAO_models[COIN]);
        modelTransformMoneda(posicionsMonedesFinals[i]);
        glDrawArrays(GL_TRIANGLES, 0, models[COIN].faces().size() * 3);
    }

    // --------- PINTEM A FANTASMA -------------
    glBindVertexArray(VAO_models[FANTASMA]);
    modelTransformFantasma();
    glDrawArrays(GL_TRIANGLES, 0, models[FANTASMA].faces().size() * 3);

    // --------- PINTEM EL LABERINT (PARETS I TERRA) -------------
    for (int i = 0; i < numFiles; i++) {
        for (int j = 0; j < numColumnes; j++) {
            if (laberint[i][j] == 1) {
                // Paret (cub 1×1×1)
                glBindVertexArray(VAO_Cub);
                modelTransformParet(glm::vec3(j, 0.0f, i));
            }
            else if (laberint[i][j] == 0) {
                // Terra (cub 1×0.1×1)
                glBindVertexArray(VAO_Cub);
                modelTransformTerra(glm::vec3(j, 0.0f, i));
            }
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // Desvinculem el VAO
    glBindVertexArray(0);
}

// Quan l'usuari canvia la mida de la finestra, actualitzem ra (aspect ratio)
void MyGLWidget::resizeGL(int ample, int alt)
{
    ample = ample;
    alt = alt;
    ra = float(ample) / float(alt);
}

// Càlcul de la matriu de visió (view) segons tipus de càmera
void MyGLWidget::viewTransform()
{
    View = glm::mat4(1.0f);

    if (primeraPersona) {
        // Càmera en primera persona: posició al cap de Morty
        glm::vec3 observadorFP = glm::vec3(posicioMorty.x, 1.3f, posicioMorty.z);
        // Direcció a on mira Morty (normalitzada)
        glm::vec3 miradaMorty = glm::vec3(
            sinf(glm::radians(angleMorty)),
            0.0f,
            cosf(glm::radians(angleMorty))
            );
        glm::vec3 amuntFP = glm::vec3(0.0f, 1.0f, 0.0f);
        View = glm::lookAt(observadorFP, observadorFP + miradaMorty, amuntFP);
    }
    else {
        // Càmera en tercera persona: orbita al voltant de VRP
        View = glm::translate(View, glm::vec3(0.0f, 0.0f, -2.0f * radiEsc));
        View = glm::rotate(View, angleY, glm::vec3(1.0f, 0.0f, 0.0f));
        View = glm::rotate(View, -angleX, glm::vec3(0.0f, 1.0f, 0.0f));
        View = glm::translate(View, -puntVisualitzacio);
    }
    // Enviem la matriu View a la GPU
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

// Càlcul de la matriu de projecció (perspectiva) segons tipus de càmera
void MyGLWidget::projectTransform()
{
    glm::mat4 Proj;

    if (primeraPersona) {
        // FOV fix de 60º, znear més proper (0.25)
        float zn_n = 0.25f;
        Proj = glm::perspective(glm::radians(60.0f), ra, zn_n, zf);
    }
    else {
        // FOV calculat a iniCamera()
        Proj = glm::perspective(fov, ra, zn, zf);
    }

    // Enviem la matriu Proj a la GPU
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

// Transformació de model per a Morty: translació, rotació i escalat
void MyGLWidget::modelTransformMorty()
{
    glm::mat4 TG(1.0f);

    // Translació a la posició món de Morty
    TG = glm::translate(TG, posicioMorty);
    // Rotació entorn de l'eix Y segons angleMorty
    TG = glm::rotate(TG, glm::radians(angleMorty), glm::vec3(0.0f, 1.0f, 0.0f));
    // Escalat uniform (1.5 × escala del model)
    TG = glm::scale(TG, glm::vec3(1.5f * escalaModels[MORTY]));
    // Translació per recentrar respecte al centreBase del model
    TG = glm::translate(TG, -centreBaseModels[MORTY]);

    // Enviem matriu a la GPU
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

// Transformació de model per al fantasma: translació i escalat
void MyGLWidget::modelTransformFantasma()
{
    glm::mat4 TG(1.0f);

    // Translació a la posició món del fantasma
    TG = glm::translate(TG, posicioFantasma);
    // Escalat uniforme (0.6 × escala del model)
    TG = glm::scale(TG, glm::vec3(0.6f * escalaModels[FANTASMA]));
    // Translació per recentrar respecte al centreBase del model
    TG = glm::translate(TG, -centreBaseModels[FANTASMA]);

    // Enviem matriu a la GPU
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

// Transformació de model per a cada moneda: translació, rotació i escalat
void MyGLWidget::modelTransformMoneda(glm::vec3 posicio)
{
    glm::mat4 TG(1.0f);

    // Translació a la posició del terra de la casella + offset (0.5,0,0.5)
    TG = glm::translate(TG, posicio);
    TG = glm::translate(TG, glm::vec3(0.5f, 0.0f, 0.5f));
    // Rotació entorn de l'eix Y segons angleMonedes
    TG = glm::rotate(TG, glm::radians(angleMonedes), glm::vec3(0.0f, 1.0f, 0.0f));
    // Escalat uniforme (0.5 × escala del model)
    TG = glm::scale(TG, glm::vec3(0.5f * escalaModels[COIN]));
    // Translació per recentrar respecte al centreBase del model
    TG = glm::translate(TG, -centreBaseModels[COIN]);

    // Enviem matriu a la GPU
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

// Transformació de model per a cada pilar (paret): translació a posició
void MyGLWidget::modelTransformParet(glm::vec3 posicio)
{
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, posicio);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

// Transformació de model per a cada casella de terra: translació i escalat pla
void MyGLWidget::modelTransformTerra(glm::vec3 posicio)
{
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, posicio);
    // Escalat només a l'alçada 0.1
    TG = glm::scale(TG, glm::vec3(1.0f, 0.1f, 1.0f));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

// Captura de tecles: moviment de Morty, canvi de càmera, focus, reset, etc.
void MyGLWidget::keyPressEvent(QKeyEvent* esdeveniment)
{
    makeCurrent(); // Fem el context actiu per assegurar-nos que els uniform canvien

    switch (esdeveniment->key()) {
    case Qt::Key_Left: {
        // Morty gira 90º a l'esquerra (augment angle)
        angleMorty += 90.0f;
        // Actualitzem direcció unitària a partir de l'angle (en radians)
        direccioMorty = glm::vec3(
            sinf(glm::radians(angleMorty)),
            0.0f,
            cosf(glm::radians(angleMorty))
            );
        // Movem el fantasma seguint Morty (respecte a la nova direcció)
        updateFantasmaPos(direccioMorty);
        // Recalculem view
        viewTransform();
        break;
    }
    case Qt::Key_Right: {
        // Morty gira 90º a la dreta (disminueix angle)
        angleMorty -= 90.0f;
        direccioMorty = glm::vec3(
            sinf(glm::radians(angleMorty)),
            0.0f,
            cosf(glm::radians(angleMorty))
            );
        updateFantasmaPos(direccioMorty);
        viewTransform();
        break;
    }
    case Qt::Key_Up: {
        // Normalitzem direcció per desplaçar-nos una unitat al laberint
        direccioMorty = glm::normalize(direccioMorty);

        // Càlcul de la nova fila i columna al laberint
        int filaNova = posicioMortyLab.x + int(direccioMorty.z);
        int colNova  = posicioMortyLab.y + int(direccioMorty.x);

        // Si dins límits i no és paret (laberint[fila][col] != 1), avancem
        if (filaNova >= 0 && filaNova < numFiles && colNova >= 0 && colNova < numColumnes &&
            laberint[filaNova][colNova] != 1) {
            posicioMorty += direccioMorty;
            posicioMortyLab.x = filaNova;
            posicioMortyLab.y = colNova;
        }
        // Actualitzem posició del fantasma cada vegada que Morty es mou
        updateFantasmaPos(direccioMorty);
        viewTransform();
        break;
    }
    case Qt::Key_C: {
        // Canvi entre càmera tercera persona i primera persona
        primeraPersona = !primeraPersona;
        emit enviaEstatCamera(primeraPersona);
        projectTransform();
        viewTransform();
        break;
    }
    case Qt::Key_L: {
        // Canvi de focus: si focusEscena = true, posem focus al Morty; si no, focus a càmera
        focusEscena = !focusEscena;
        enviaPosFocus();
        emit enviaEstatFocus(focusEscena);
        break;
    }
    case Qt::Key_R: {
        // Reset de tota l'escena: posicions, càmera, monedes i focus
        angleX = angleY = M_PI / 4.0f;
        focusEscena = true;
        primeraPersona = false;

        // Reset de Morty
        angleMorty = 0.0f;
        posicioMorty = glm::vec3(1.5f, 0.0f, 1.5f);
        posicioMortyLab = glm::vec2(1, 1);
        direccioMorty = glm::vec3(0.0f, 0.0f, 1.0f);

        // Reset de fantasma
        angleFantasma = 0.0f;
        posicioFantasma = glm::vec3(8.5f, 0.0f, 10.5f);
        posicioFantasmaLab = glm::vec2(9, 7);
        direccioFantasma = glm::vec3(-1.0f, 0.0f, 0.0f);

        // Buida el vector de monedes existents i marquem totes posicionsDisponibles com a false
        posicionsMonedesFinals.clear();
        for (auto &elem : posicionsDisponibles) {
            elem.second = false;
        }

        // Recalcul de noves posicions per a 10 monedes aleatòries
        int numMaxMonedes = 10;
        int comptMonedes = 0;
        while (comptMonedes < numMaxMonedes) {
            int idxAleatori = std::rand() % posicionsDisponibles.size();
            if (posicionsDisponibles[idxAleatori].second == false) {
                posicionsMonedesFinals.push_back(posicionsDisponibles[idxAleatori].first);
                posicionsDisponibles[idxAleatori].second = true;
                ++comptMonedes;
            }
        }

        // Recalculem les matrius de càmera i focus
        projectTransform();
        viewTransform();
        enviaPosFocus();
        break;
    }
    default:
        // Per qualsevol altra tecla, deleguem al pare
        ExamGLWidget::keyPressEvent(esdeveniment);
        break;
    }

    update(); // Repaint
}

// Captura del moviment de ratolí per rotar la càmera (només si no estem en primera persona)
void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();
    if (DoingInteractive == ROTATE && !primeraPersona)
    {
        // Rotem segons diferència de posició del ratolí
        angleX -= (e->x() - xClick) * (M_PI / 360.0f);
        angleY += (e->y() - yClick) * (M_PI / 360.0f);
        viewTransform();
    }

    // Emmagatzemem la nova posició del clic
    xClick = e->x();
    yClick = e->y();

    update();
}

// Actualitza la posició del fantasma en funció de la direcció de Morty
void MyGLWidget::updateFantasmaPos(glm::vec3 direccioMorty)
{
    // Determinem angleFantasma segons direcció de Morty:
    if (int(direccioMorty.z) == 1)  angleFantasma = M_PI;
    if (int(direccioMorty.z) == -1) angleFantasma = 0.0f;
    if (int(direccioMorty.x) == 1)  angleFantasma = -M_PI / 2.0f;
    if (int(direccioMorty.x) == -1) angleFantasma = M_PI / 2.0f;

    // Vector unitari de direcció del fantasma a partir de l'angle
    direccioFantasma = glm::vec3(
        sinf(angleFantasma),
        0.0f,
        cosf(angleFantasma)
        );
    // Desplacem el fantasma una unitat en aquesta direcció
    posicioFantasma += direccioFantasma;

    // Si surt dels límits (0..15 en x o z), el fem retrocedir
    if (posicioFantasma.x > 15.0f || posicioFantasma.x < 0.0f ||
        posicioFantasma.z > 15.0f || posicioFantasma.z < 0.0f) {
        posicioFantasma -= direccioFantasma;
    }
}

// Slot cridat cada 80 ms per girar les monedes (si girant == true)
void MyGLWidget::girarMonedes()
{
    if (girant) {
        makeCurrent();        // Context OpenGL actiu
        angleMonedes += 20.0f; // Augment de l'angle per fer l'efecte
        update();             // Repaint perquè es vegin girar
    }
}

// Carrega els shaders i obté la ubicació del uniform "posFoc"
void MyGLWidget::carregaShaders()
{
    ExamGLWidget::carregaShaders();
    // Ubicació del uniform que indica la posició del focus
    locPosicioFocus = glGetUniformLocation(program->programId(), "posFoc");
}

// --------------------------------------------------------------
// Metodes nous per a l'alumne (útils per càlculs d'escena)
// --------------------------------------------------------------

// Calcula centreEscena i radiEscena de la capsa contenidora donada pMin, pMax
void MyGLWidget::creaCapsaContenidora(glm::vec3 pMin, glm::vec3 pMax)
{
    centreEsc = (pMax + pMin) / glm::vec3(2.0f, 2.0f, 2.0f);
    radiEsc = sqrtf(
                     powf(pMax.x - pMin.x, 2.0f) +
                     powf(pMax.y - pMin.y, 2.0f) +
                     powf(pMax.z - pMin.z, 2.0f)
                     ) / 2.0f;
}

// Envia la posició del focus (vector4) a la GPU segons focusEscena
void MyGLWidget::enviaPosFocus()
{
    if (!focusEscena) {
        // Focus a la posició de Morty (coordenades món), però convertim a coords càmera multiplicant per View
        posicioFocus = View * glm::vec4(posicioMorty.x, 1.0f, posicioMorty.z, 1.0f);
    }
    else {
        // Focus fixe a l'origen de l'escena (0,0,0,1)
        posicioFocus = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    // Enviem el vector del focus (x,y,z) a la GPU
    glUniform3fv(locPosicioFocus, 1, &posicioFocus[0]);
}

// Atura el gir de monedes, pari el timer
void MyGLWidget::aturar() {
    girant = false;
    timerCoins.stop();
}

// Engega (reactiva) el gir de monedes
void MyGLWidget::engegar() {
    girant = true;
    timerCoins.start(80);
}

// Activa la càmera en primera persona
void MyGLWidget::setCamPrimeraPersona()
{
    makeCurrent();
    if (!primeraPersona)
        primeraPersona = true;
    projectTransform();
    viewTransform();
    update();
}

// Activa la càmera en tercera persona (escena)
void MyGLWidget::setCamEscena()
{
    makeCurrent();
    if (primeraPersona)
        primeraPersona = false;
    projectTransform();
    viewTransform();
    update();
}

// Envia posFocus fix a càmera (quan focusEscena == true)
void MyGLWidget::setFocusCamera()
{
    makeCurrent();
    if (!focusEscena)
        focusEscena = true;
    enviaPosFocus();
    update();
}

// Envia posFocus al cap de Morty (quan focusEscena == false)
void MyGLWidget::setFocusMorty()
{
    makeCurrent();
    if (focusEscena)
        focusEscena = false;
    enviaPosFocus();
    update();
}

// Reset de l'escena, monedes, càmera i focus (es fa el mateix que en Key_R)
void MyGLWidget::reset()
{
    makeCurrent();
    angleX = angleY = M_PI / 4.0f;
    focusEscena = true;
    primeraPersona = false;

    // Morty
    angleMorty = 0.0f;
    posicioMorty = glm::vec3(1.5f, 0.0f, 1.5f);
    posicioMortyLab = glm::vec2(1, 1);
    direccioMorty = glm::vec3(0.0f, 0.0f, 1.0f);

    // Fantasma
    angleFantasma = 0.0f;
    posicioFantasma = glm::vec3(8.5f, 0.0f, 10.5f);
    posicioFantasmaLab = glm::vec2(9, 7);
    direccioFantasma = glm::vec3(-1.0f, 0.0f, 0.0f);

    // Buida i recalcula monedes
    posicionsMonedesFinals.clear();
    for (auto &elem : posicionsDisponibles) {
        elem.second = false;
    }
    int numMaxMonedes = 10;
    int comptMonedes = 0;
    while (comptMonedes < numMaxMonedes) {
        int idxAleatori = std::rand() % posicionsDisponibles.size();
        if (posicionsDisponibles[idxAleatori].second == false) {
            posicionsMonedesFinals.push_back(posicionsDisponibles[idxAleatori].first);
            posicionsDisponibles[idxAleatori].second = true;
            ++comptMonedes;
        }
    }

    projectTransform();
    viewTransform();
    enviaPosFocus();

    update();
}
