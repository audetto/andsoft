#ifndef DIVX_H
#define DIVX_H

#include <qmainwindow.h>

#include "dec.h"
#include "enc.h"

class ApplicationWindow: public QMainWindow {
    Q_OBJECT
public:
    ApplicationWindow();
protected:
    void closeEvent(QCloseEvent *ce);
private slots:
    void about();
    void aboutQt();
private:
    DivX_Encode *en;
    DivX_Decode *de;
    ScreenX *sx;
};

#endif
