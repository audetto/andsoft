#ifndef NETMONITOR_H
#define NETMONITOR_H

#include <qlistview.h>
#include <qmainwindow.h>
#include <qstring.h>

#include <map>
#include <string>

using namespace std;

struct Dati {
  int rstart;
  int tstart;
  int rlast;
  int tlast;
  int rmaximum;
  int tmaximum;
  QListViewItem *ritem;
  QListViewItem *titem;
  double timestart;
  bool active;
};

typedef map<QString, Dati*> Tabella;

class NetMonitor: public QMainWindow {

  Q_OBJECT

public:
  NetMonitor();
  ~NetMonitor();
public slots:
  void update();
private:
  void extract(string &riga, QString &name, int &rbytes, int &tbytes);
  int getInt(string &riga);

  double lasttime;
  Tabella tabella;
  QListView *rec;
  QListView *tra;
};

#endif
