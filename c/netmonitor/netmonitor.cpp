#include <qapplication.h>
#include <qtabwidget.h>
#include <qtimer.h>

#include <fstream>

#include <sys/time.h>

#include "netmonitor.h"

NetMonitor::NetMonitor() 
  : QMainWindow(NULL, NULL, WDestructiveClose )
{
  timeval tv;
  gettimeofday(&tv, NULL);

  lasttime = tv.tv_sec;

  setCaption("Net Monitor - (C)opyRight AndSoft Inc., 2003-04");
  
  QTabWidget *qt = new QTabWidget(this);

  rec = new QListView(qt);
  rec->addColumn("Interface");
  rec->addColumn("Bytes");
  rec->setColumnAlignment(1, Qt::AlignRight);
  rec->addColumn("Speed");
  rec->setColumnAlignment(2, Qt::AlignRight);
  rec->addColumn("Current");
  rec->setColumnAlignment(3, Qt::AlignRight);
  rec->addColumn("Maximum");
  rec->setColumnAlignment(4, Qt::AlignRight);

  //  connect(rec, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(showgauge(QListViewItem *)));
  qt->addTab(rec, "Receive");

  tra = new QListView(qt);
  tra->addColumn("Interface");
  tra->addColumn("Bytes");
  tra->setColumnAlignment(1, Qt::AlignRight);
  tra->addColumn("Speed");
  tra->setColumnAlignment(2, Qt::AlignRight);
  tra->addColumn("Current");
  tra->setColumnAlignment(3, Qt::AlignRight);
  tra->addColumn("Maximum");
  tra->setColumnAlignment(4, Qt::AlignRight);

  qt->addTab(tra, "Transmit");

  setCentralWidget(qt);

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(2000, FALSE);
}

NetMonitor::~NetMonitor() {
  for (Tabella::iterator it = tabella.begin(); it != tabella.end(); it++) {
    Dati* dato = (*it).second;
    delete dato;  
    // non li tolgo dal "map" perche' tanto lo fa il distruttore e nessuno usera' piu' il puntatore
  }
}

int NetMonitor::getInt(string &riga) {
  static const string CIFRE("0123456789");
  riga.erase(0, riga.find_first_of(CIFRE));
  int a = riga.find_first_not_of(CIFRE);
  string temp = riga.substr(0, a);
  riga.erase(0, a);
  return atoi(temp.c_str());
}

void NetMonitor::extract(string &riga, QString &name, int &rbytes, int &tbytes) {
  QString value = riga.substr(0, 6).c_str();
  name = value.simplifyWhiteSpace();
  riga.erase(0, 7);

  rbytes = getInt(riga);
  getInt(riga);
  getInt(riga);
  getInt(riga);
  getInt(riga);
  getInt(riga);
  getInt(riga);
  getInt(riga);
  tbytes = getInt(riga);
}

void NetMonitor::update() {
  ifstream dev("/proc/net/dev");
  char buf[255];

  timeval tv;
  gettimeofday(&tv, NULL);
  
  double actualtime = tv.tv_sec + tv.tv_usec / 1000000.0;
  
  Tabella::iterator it;
  for (it = tabella.begin(); it != tabella.end(); it++) {
    Dati* dato = (*it).second;
    dato->active = false;
  }

  dev.getline(buf, 250);
  dev.getline(buf, 250);

  while (dev.good() && !dev.eof()) {
    dev.getline(buf, 250);
    string riga(buf);
    if (riga.length() > 50) {
      QString name;
      int rbytes, tbytes;
      extract(riga, name, rbytes, tbytes);
      it = tabella.find(name);
      Dati *dato;
      if (it != tabella.end()) {
	dato = (*it).second;
	double et = actualtime - dato->timestart;
	if (et > 0) {
	  int rspeed = int((rbytes - dato->rstart) / et);
	  int tspeed = int((tbytes - dato->tstart) / et);
	  dato->ritem->setText(2, QString::number(rspeed)); 
	  dato->titem->setText(2, QString::number(tspeed));
	}
	et = actualtime - lasttime;
	if (et > 0) {
	  int rspeed = int((rbytes - dato->rlast) / et);
	  int tspeed = int((tbytes - dato->tlast) / et);
	  dato->ritem->setText(3, QString::number(rspeed)); 
	  dato->titem->setText(3, QString::number(tspeed));
	  if (rspeed > dato->rmaximum) {
	    dato->ritem->setText(4, QString::number(rspeed));
	    dato->rmaximum = rspeed;
	  }
	  if (tspeed > dato->tmaximum) {
	    dato->titem->setText(4, QString::number(tspeed));
	    dato->tmaximum = tspeed;
	  }
	}
      } else {
	dato = new Dati;

	dato->ritem = new QListViewItem(rec, name);
	dato->titem = new QListViewItem(tra, name);

	dato->rstart = rbytes;
	dato->tstart = tbytes;
	dato->rmaximum = 0;
	dato->tmaximum = 0;
	dato->timestart = actualtime;
	tabella[name] = dato;
      }
      dato->rlast = rbytes;
      dato->tlast = tbytes;
      dato->active = true;
      dato->ritem->setText(1, QString::number(rbytes)); 
      dato->titem->setText(1, QString::number(tbytes)); 
    }
  }

  for (it = tabella.begin(); it != tabella.end(); it++) {
    Dati* dato = (*it).second;
    if (!dato->active) {
      delete dato;
      tabella.erase(it); // forse il iterator non e' piu' valido dopo questa chiamata...
    }
  }

  lasttime = actualtime;
}

int main(int argc, char **argv) {
  QApplication a(argc, argv);		// create application object
  QWidget *nm = new NetMonitor;
  nm->show();
  a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
  return a.exec();
}
