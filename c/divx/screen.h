#ifndef SCREEN_H
#define SCREEN_H

#include <qwidget.h>
#include <qthread.h>

class ScreenX : public QWidget {

  Q_OBJECT

public:
    ScreenX( QWidget *parent = 0, const char *name = 0 );
    bool getUso();
    void setUso(bool uso);
protected:
    void paintEvent(QPaintEvent *);
public:
    QMutex mutex;
    bool presente;
    bool inuso;
    int height;
    int width;
    unsigned char *img2;
    unsigned char *image_rgb;
};

#endif
