#include "ui_MyForm.h"

class MyForm : public QWidget
{
  Q_OBJECT

  public:
    MyForm (QWidget *parent=0);
  
    static const int CLOCK_PERIOD_MILIS=50;


  private:
    Ui::MyForm ui;
    QTimer *timer ;
    bool modeManual=true;
  public slots:
    void tooglePlay(bool a);
    void setModeManual(bool esManual);

};
