TEMPLATE    = app
QT         += opengl

INCLUDEPATH +=  /home/david/INDI/glm-master
INCLUDEPATH += ./Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h \
    A3GLWidget.h

SOURCES += main.cpp  MyForm.cpp \
        MyGLWidget.cpp ./Model/model.cpp \
        A3GLWidget.cpp


DISTFILES += \
    shaders/basicLlumShader.frag \
    shaders/basicLlumShader.vert

RESOURCES += shaders.qrc

