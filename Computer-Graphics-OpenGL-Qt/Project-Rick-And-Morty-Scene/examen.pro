TEMPLATE    = app
QT         += opengl


INCLUDEPATH += ./include
INCLUDEPATH += ./Model


FORMS += MyForm.ui

HEADERS += MyForm.h ExamGLWidget.h MyGLWidget.h

SOURCES += main.cpp  MyForm.cpp ./Model/model.cpp \
        ExamGLWidget.cpp MyGLWidget.cpp 

RESOURCES += shaders.qrc
