#include <qpainter.h>
#include <qimage.h>

#include "screen.h"
#include "divx_asi.h"
#include "ccvt.h"

ScreenX::ScreenX( QWidget *parent, const char *name )
  : QWidget( parent, name, Qt::WRepaintNoErase ) {
  presente = false;
  inuso = false;
  setMinimumSize(RW, RH);
  setMaximumSize(RW, RH);
}

void ScreenX::paintEvent(QPaintEvent *) {
  if (presente) {
    mutex.lock();
    ccvt_420p_bgr32(width, height, img2, image_rgb);
    mutex.unlock();

    QImage img(image_rgb, width, height, 32, NULL, 0, QImage::LittleEndian);
    QPainter paint(this);
    paint.drawImage(0, 0, img);
  } 
}

bool ScreenX::getUso() {
  bool uso;
  mutex.lock();
  uso = inuso;
  mutex.unlock();
  return uso;
}

void ScreenX::setUso(bool uso) {
  mutex.lock();
  inuso = uso;
  mutex.unlock();
}
