#include <qapplication.h>
#include <qtabwidget.h>
#include <qtimer.h>
#include <qvbox.h>

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

  QVBox *vbox = new QVBox(qt);

  combo = new QComboBox(false, vbox);
  rdial = new QProgressBar(vbox);
  tdial = new QProgressBar(vbox);
  
  qt->addTab(vbox, "Dial");

  setCentralWidget(qt);

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(2000, FALSE);
}

NetMonitor::~NetMonitor() {
  for (Table::iterator it = table.begin(); it != table.end(); table.erase(it++)) {
    InterfaceData* interface = (*it).second;
    delete interface->ritem;
    delete interface->titem;
    delete interface;
  }
}

int NetMonitor::getInt(string &line) const {
  static const string CIFRE("0123456789");
  line.erase(0, line.find_first_of(CIFRE));
  const int a = line.find_first_not_of(CIFRE);
  string temp = line.substr(0, a);
  line.erase(0, a);
  return atoi(temp.c_str());
}

void NetMonitor::extract(string &line, QString &name, int &rbytes, int &tbytes) const {
  const QString value = line.substr(0, 6).c_str();
  name = value.simplifyWhiteSpace();
  line.erase(0, 7);

  rbytes = getInt(line);
  getInt(line);
  getInt(line);
  getInt(line);
  getInt(line);
  getInt(line);
  getInt(line);
  getInt(line);
  tbytes = getInt(line);
}

void NetMonitor::update() {
  ifstream dev("/proc/net/dev");
  char buf[255];

  timeval tv;
  gettimeofday(&tv, NULL);
  
  const double actualtime = tv.tv_sec + tv.tv_usec / 1000000.0;
  
  for (Table::iterator it = table.begin(); it != table.end(); it++) {
    InterfaceData* interface = (*it).second;
    interface->active = false;
  }

  dev.getline(buf, 250);
  dev.getline(buf, 250);

  while (dev.good() && !dev.eof()) {
    dev.getline(buf, 250);
    string line(buf);
    QString selezionato = combo->currentText();
    if (line.length() > 50) {
      QString name;
      int rbytes, tbytes;
      extract(line, name, rbytes, tbytes);
      Table::iterator it = table.find(name);
      InterfaceData *interface;
      if (it != table.end()) {
	interface = (*it).second;
	const double ett = actualtime - interface->timestart;
	if (ett > 0) {
	  const int rspeed = int((rbytes - interface->rstart) / ett);
	  const int tspeed = int((tbytes - interface->tstart) / ett);
	  interface->ritem->setText(2, QString::number(rspeed)); 
	  interface->titem->setText(2, QString::number(tspeed));
	}
	const double elt = actualtime - lasttime;
	if (elt > 0) {
	  const int rspeed = int((rbytes - interface->rlast) / elt);
	  const int tspeed = int((tbytes - interface->tlast) / elt);
	  interface->ritem->setText(3, QString::number(rspeed)); 
	  interface->titem->setText(3, QString::number(tspeed));
	  if (rspeed > interface->rmaximum) {
	    interface->ritem->setText(4, QString::number(rspeed));
	    interface->rmaximum = rspeed;
	  }
	  if (tspeed > interface->tmaximum) {
	    interface->titem->setText(4, QString::number(tspeed));
	    interface->tmaximum = tspeed;
	  }
	  if (name == selezionato) {
	    rdial->setProgress(rspeed, interface->rmaximum);
	    tdial->setProgress(tspeed, interface->tmaximum);
	  }
	}
      } else {
	combo->insertItem(name);

	interface = new InterfaceData;

	interface->ritem = new QListViewItem(rec, name);
	interface->titem = new QListViewItem(tra, name);

	interface->rstart = rbytes;
	interface->tstart = tbytes;
	interface->rmaximum = 0;
	interface->tmaximum = 0;
	interface->timestart = actualtime;
	table[name] = interface;
      }
      interface->rlast = rbytes;
      interface->tlast = tbytes;
      interface->active = true;
      interface->ritem->setText(1, QString::number(rbytes)); 
      interface->titem->setText(1, QString::number(tbytes)); 
    }
  }

  for (Table::iterator it = table.begin(); it != table.end(); it++) {
    InterfaceData* interface = (*it).second;
    if (!interface->active) {
      delete interface->ritem;
      delete interface->titem;
      delete interface;
      table.erase(it);
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
