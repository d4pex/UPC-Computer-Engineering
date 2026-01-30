TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /home/david/INDI/glm-master  \
                ./Model 

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
           BL2GLWidget.cpp MyGLWidget.cpp  \
           ./Model/model.cpp

RESOURCES += shaders.qrc
