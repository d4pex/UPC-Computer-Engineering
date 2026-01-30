TEMPLATE    = app
QT         += opengl

INCLUDEPATH +=  /home/david/INDI/glm-master
INCLUDEPATH += ./Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp  MyForm.cpp ./Model/model.cpp \
        MyGLWidget.cpp 

RESOURCES += shaders.qrc
