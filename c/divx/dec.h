#ifndef DEC_H
#define DEC_H

#include <qwidget.h>
#include <qthread.h>

#include "screen.h"

class DivX_Decode : public QObject, public QThread {

  Q_OBJECT

public:
    DivX_Decode(ScreenX *scx, QObject *parent = 0, const char *name = 0);
public slots:
    void doit();
    void done();
protected:
    void run();
private:
    bool continua;
    QString file_name;
    void show_msg(const QString &msg);
    ScreenX *sx;
};

#endif
