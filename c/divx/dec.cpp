#include <qapplication.h>
#include <qmessagebox.h>
#include <qinputdialog.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include <decore.h>

#include <vector>

#include "dec.h"
#include "divx_asi.h"

DivX_Decode::DivX_Decode(ScreenX *scx, QObject *parent, const char *name)
    : QObject( parent, name )
{
  sx = scx;
  continua = false;
}

void DivX_Decode::show_msg(const QString &msg) {
  qApp->lock();
  QMessageBox::warning(NULL, "DivX Decoder", msg, "Ok");
  qApp->unlock();
}

void DivX_Decode::run() {

  int fd = ::open(file_name + ".inf", O_RDONLY);
  if (fd < 0)
    show_msg(strerror(errno));
  else {

    divx_header header;
    int res = ::read(fd, &header, sizeof(header));
    if (res != sizeof(header) || header.magic != MAGIC)
      show_msg("Errore nella lettura dell'header!");
    else {
      std::vector<int> frame_lengths(header.frames);
      res = ::read(fd, frame_lengths.begin(), sizeof(int) * header.frames);
      ::close(fd);
      if (res != sizeof(int) * header.frames)
	show_msg("Errore nella lettura dell'header (2)!");
      else {
  
	fd = ::open(file_name + ".divx", O_RDONLY);
	if (fd < 0)
	  show_msg(strerror(errno));
	else {
	  
	  sx->height = header.height;
	  sx->width = header.width;

	  void *decore_handle;
	  res = decore(&decore_handle, DEC_OPT_INIT, NULL, NULL);

	  DivXBitmapInfoHeader format;
	  memset(&format, 0, sizeof(DivXBitmapInfoHeader));
	  format.biSize = sizeof(DivXBitmapInfoHeader);
	  format.biWidth = header.width;
	  format.biHeight = -header.height;
	  //	  format.biCompression = 0;
	  //	  format.biBitCount = 32;
	  format.biCompression = FOURCC('I', '4', '2', '0');

	  res = decore(decore_handle, DEC_OPT_SETOUT, &format, NULL);

	  int size_yuv = header.height * header.width * 3 / 2;
	  int size_rgb = header.height * header.width * 4;
	  int size_divx = header.height * header.width * 6;
	  
	  unsigned char *image_yuv = new unsigned char [size_yuv];
	  sx->image_rgb = new unsigned char [size_rgb];
	  sx->img2 = new unsigned char [size_yuv];
	  unsigned char *stream = new unsigned char [size_divx];
	  DEC_FRAME dec_frame;
	  DEC_FRAME dec_frame_info;

	  bool my_continua;

	  for (int frame = 0; frame < header.frames; frame++) {
	    memset(&dec_frame, 0, sizeof(dec_frame));
	    dec_frame.bmp = image_yuv;
	    dec_frame.bitstream = stream;
	    dec_frame.render_flag = 1;
	    dec_frame.length = frame_lengths[frame];
	    res = ::read(fd, stream, frame_lengths[frame]);
	    if (res != frame_lengths[frame]) {
	      show_msg("Errore nella lettura dei dati!");
	      my_continua = false;
	    } else {
	      res = decore(decore_handle, DEC_OPT_FRAME, &dec_frame, &dec_frame_info);
	      if (res != DEC_OK) {
		show_msg("Errore nella decodifica dei dati!");
		my_continua = false;
	      } else {
		sx->mutex.lock();
		sx->presente = true;
		memcpy(sx->img2, image_yuv, size_yuv); 
		my_continua = continua;
		sx->mutex.unlock();

		QThread::postEvent(sx, new QPaintEvent(QRect(0, 0, header.width, header.height), false));
	      }
	    }
	    if (my_continua == false) break;
	  }

	  sx->mutex.lock();
	  sx->presente = false;
	  sx->mutex.unlock();

	  delete [] image_yuv;
	  delete [] stream;
	  delete [] sx->image_rgb;
	  delete [] sx->img2;

	  sx->image_rgb = NULL;
	  sx->img2 = NULL;
	}
      }
    }
  }
  sx->setUso(false);
}

void DivX_Decode::doit() {
  if (sx->getUso()) {
    QMessageBox::warning(NULL, "DivX Decoder", "Attendere la fine delle operazioni", "Ok");
  } else {
    sx->setUso(true);
    file_name = QInputDialog::getText("Immettere nome file", "File", "film");
    continua = true;
    start();
  }
}

void DivX_Decode::done() {
  sx->mutex.lock();
  continua = false;
  sx->mutex.unlock();
}
