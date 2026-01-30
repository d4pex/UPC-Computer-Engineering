#include "MyForm.h"
#include <QTimer>
MyForm::MyForm (QWidget* parent) : QWidget(parent)
{
  ui.setupUi(this);

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, ui.myGLWidget, &MyGLWidget::tick);
  connect(ui.btnPlay, &QPushButton::clicked, this, &MyForm::tooglePlay);
  setModeManual(true);
}

void MyForm::tooglePlay(bool a)
{
    if(!modeManual){
        if(timer->isActive()) {
            timer->stop();
            ui.btnPlay->setText("Play");
        } else {
            timer->start(MyForm::CLOCK_PERIOD_MILIS);
            ui.btnPlay->setText("Pause");
        }
    }
}

void MyForm::setModeManual(bool esManual){

    modeManual=esManual;

    ui.myGLWidget->resetTime();
    ui.myGLWidget->setModeManual(esManual);
    ui.myGLWidget->update();
    if(modeManual){
        timer->stop();
        ui.btnPlay->setEnabled(false);
        ui.btnPlay->setText("<>");
    } else {
        timer->start(MyForm::CLOCK_PERIOD_MILIS);
        ui.btnPlay->setEnabled(true);
        ui.btnPlay->setText("Pause");
    }
}


