#include <qapplication.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qmenubar.h>

#include <encore.h>
#include <decore.h>

#include <stdio.h>

#include "divx_asi.h"
#include "divx.h"

ApplicationWindow::ApplicationWindow() 
  : QMainWindow(NULL, NULL, WDestructiveClose )
{
  setCaption("DivX Coder - (C)opyRight AndSoft Inc., 2003");

  sx = new ScreenX(this);

  en = new DivX_Encode(sx, this);
  de = new DivX_Decode(sx, this);

  QPopupMenu *system = new QPopupMenu(this);
  menuBar()->insertItem("&System", system);
  system->insertItem("&Quit", qApp, SLOT(closeAllWindows()), CTRL+Key_Q);

  QPopupMenu *camera = new QPopupMenu(this);
  menuBar()->insertItem("&Camera", camera);
  camera->insertItem("&Start", en, SLOT(doit()));
  camera->insertItem("S&top", en, SLOT(done()));

  QPopupMenu *player = new QPopupMenu(this);
  menuBar()->insertItem("&Player", player);
  player->insertItem("&Start", de, SLOT(doit()));
  player->insertItem("S&top", de, SLOT(done()));

  QPopupMenu *help = new QPopupMenu(this);
  menuBar()->insertItem("&Help", help);
  help->insertItem("&About", this, SLOT(about()), Key_F1);
  help->insertItem("About &Qt", this, SLOT(aboutQt()));

  setCentralWidget(sx);
}

void ApplicationWindow::closeEvent(QCloseEvent *ce) {
  if (en->running() || de->running() || sx->getUso()) {
    QMessageBox::warning(this, "DivX Coder", "Attendere la fine delle operazioni", "Ok");
    ce->ignore();
  } else {
    ce->accept();
  }
}

void ApplicationWindow::about() {
  int divx_version = ENCORE_MAJOR_VERSION;
  int enc_version = encore(NULL, ENC_OPT_VERSION, NULL, NULL);
  int dec_version = decore(NULL, DEC_OPT_VERSION, NULL, NULL);
  int enc_include = ENCORE_VERSION;
  int dec_include = DECORE_VERSION;
  char buf[254];

  sprintf(buf, "(C)opyRight AndSoft Inc., 2003\n\n" 
	       "DivX Version: %d\n"
	       "Encode Library: %d (%d)\n"
	       "Decode Library: %d (%d)", 
	  divx_version, enc_version, enc_include, dec_version, dec_include);

  QMessageBox::about(this, "DivX Coder", buf);
}
    
void ApplicationWindow::aboutQt() {
  QMessageBox::aboutQt(this, "About Qt");
}

int main(int argc, char **argv) {
  QApplication a(argc, argv);		// create application object
  ApplicationWindow * mw = new ApplicationWindow();
  mw->show();
  a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
  return a.exec();
}
