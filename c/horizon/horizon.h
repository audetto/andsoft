#ifndef HORIZON_H
#define HORIZON_H

#include <qlineedit.h>
#include <qmainwindow.h>

class Orizzonte : public QMainWindow {
  Q_OBJECT
public:
    Orizzonte();
public slots:
    void go();
    void aboutQt();
private:
    QLineEdit *distanza;
    QLineEdit *nascosta;
    QLineEdit *altezza;
    QLineEdit *sporgenza;
    QLineEdit *osservatore;
    QLineEdit *angolo;
    QLineEdit *orizzonte;
    QLineEdit *raggio;
    QLineEdit *note;
    QLineEdit *confronto;
    QLineEdit *equivalente;
};

#endif
