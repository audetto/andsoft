#include <qapplication.h>
#include <qmessagebox.h>
#include <qinputdialog.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <linux/videodev.h>

#include <encore.h>

#include <vector>

#include "enc.h"
#include "divx_asi.h"

DivX_Encode::DivX_Encode(ScreenX *scx, QObject *parent, const char *name)
    : QObject(parent, name)
{
  sx = scx;
  continua = false;
}

void DivX_Encode::show_msg(const QString &msg) {
  qApp->lock();
  QMessageBox::warning(NULL, "DivX Encoder", msg, "Ok");
  qApp->unlock();
}

void DivX_Encode::run() {
  int CamFD;

  CamFD = open("/dev/video0", O_RDONLY);

  if (CamFD >= 0) {
    int res;
    
    struct video_window VWin;

    memset(&VWin, 0, sizeof(VWin));
    VWin.width = RW;
    VWin.height = RH;
    res = ioctl(CamFD, VIDIOCSWIN, &VWin);
    if (res < 0)
      show_msg(strerror(errno));
    else {

      res = ioctl(CamFD, VIDIOCGWIN, &VWin);
      //    if (res < 0) ok = false;

      sx->height = VWin.height;
      sx->width  = VWin.width;

      int size_yuv = VWin.height * VWin.width * 3 / 2;
      int size_rgb = VWin.height * VWin.width * 4;
      int size_divx = VWin.height * VWin.width * 6;
      unsigned char *image_yuv = new unsigned char [size_yuv];
      sx->image_rgb = new unsigned char [size_rgb];
      sx->img2 = new unsigned char [size_yuv];

      unsigned char *stream; 

      void* encore_handle;
      DivXBitmapInfoHeader format;
      SETTINGS settings;
      ENC_FRAME enc_frame;
      ENC_RESULT enc_result;
      divx_header header;

      std::vector<int> frame_lengths;

      bool my_continua;

      int fd;
      if (file_name.isEmpty())
	fd = -1;
      else {
	fd = open(file_name + ".divx", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd < 0)
	  show_msg(strerror(errno));
	else {
	  stream = new unsigned char [size_divx]; 
	  memset(&settings, 0, sizeof(settings));
	  memset(&format, 0, sizeof(format));
	  memset(&enc_frame, 0, sizeof(enc_frame));

	  format.biSize = sizeof(DivXBitmapInfoHeader);
	  format.biWidth = VWin.width;
	  format.biHeight = VWin.height;
	  format.biCompression = FOURCC('I', '4', '2', '0');

	  header.width = VWin.width;
	  header.height = VWin.height;
	  header.frames = 0;

	  res = encore(&encore_handle, ENC_OPT_INIT, &format, &settings);
      
	}
      }

      timeval t0;
      struct timezone tz;
      gettimeofday(&t0, &tz);

      // MMAP operations
      /*      struct video_mbuf MBuf;
      res = ioctl(CamFD, VIDIOCGMBUF, &MBuf);
      
      void *image_yuv2 = mmap(NULL, MBuf.size, PROT_READ, MAP_PRIVATE, CamFD, 0); */

      do {
	res = read(CamFD, image_yuv, size_yuv);

	sx->mutex.lock();
	sx->presente = true;
        memcpy(sx->img2, image_yuv, size_yuv);
	my_continua = continua;
	sx->mutex.unlock();

	QThread::postEvent(sx, new QPaintEvent(QRect(0, 0, VWin.width, VWin.height), false));

	if (fd >= 0) {
	  enc_frame.image = image_yuv;
	  enc_frame.bitstream = stream;
	  enc_frame.produce_empty_frame = 0;

	  int current_size = 0;

	  do {
	    res = encore(encore_handle, ENC_OPT_ENCODE, &enc_frame, &enc_result);
	    unsigned char *tmp = ((unsigned char *)enc_frame.bitstream) + enc_frame.length;
	    enc_frame.bitstream = tmp;
	    if (enc_result.cType != '\0') current_size += enc_frame.length;
	    enc_frame.image = NULL;
	  } while (enc_frame.length > 0);

	  frame_lengths.push_back(current_size);
	  res = write(fd, stream, current_size);
	} else
	  frame_lengths.push_back(0);

      } while (my_continua);

      //      munmap(image_yuv2, MBuf.size);

      timeval t1;
      gettimeofday(&t1, &tz);

      qApp->lock();
      sx->presente = false;
      if (!frame_lengths.empty()) {
	double fps = (double)(frame_lengths.size()) / (t1.tv_sec - t0.tv_sec);
	QMessageBox::information(NULL, "FPS", QString::number(fps, 'g', 4) + QString(" frames per second!"), "Ok");
      }
      qApp->unlock();

      delete [] sx->img2;
      sx->img2 = NULL;
      delete [] image_yuv;
      delete [] sx->image_rgb;
      sx->image_rgb = NULL;

      if (fd >= 0) {
	delete [] stream;
	res = encore(encore_handle, ENC_OPT_RELEASE, NULL, NULL);
	::close(fd);

	fd = open(file_name + ".inf", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd < 0) 
	  show_msg(strerror(errno));
	else {
	  header.magic = MAGIC;
	  header.frames = frame_lengths.size();
	  res = write(fd, &header, sizeof(header));
	  res = write(fd, frame_lengths.begin(), sizeof(int) * header.frames);
	  ::close(fd);
	}
      }
    }
    ::close(CamFD);
  } else
    show_msg(strerror(errno));

  sx->setUso(false);
}

void DivX_Encode::doit() {
  if (sx->getUso()) {
    QMessageBox::warning(NULL, "DivX Encoder", "Attendere la fine delle operazioni", "Ok");
  } else {
    sx->setUso(true);
    file_name = QInputDialog::getText("Immettere nome file", "File", "film");
    continua = true;
    start();
  }
}

void DivX_Encode::done() {
  sx->mutex.lock();
  continua = false;
  sx->mutex.unlock();
}
