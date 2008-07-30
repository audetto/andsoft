#ifndef NETMONITOR_H
#define NETMONITOR_H

#include <QListView>
#include <QMainWindow>
#include <QString>
#include <QProgressBar>
#include <QComboBox>
#include <QTableWidget>

#include <map>
#include <string>

using namespace std;

struct InterfaceData {
  double rstart;
  double tstart;
  double rlast;
  double tlast;
  int rmaximum;
  int tmaximum;
  int row;
  double timestart;
  bool active;
};

typedef map<QString, InterfaceData*> Table_t;

class NetMonitor: public QMainWindow {

  Q_OBJECT

public:
  NetMonitor();
  ~NetMonitor();
public slots:
  void update();
private:
  void extract(string &line, QString &name, double &rbytes, double &tbytes) const;
  double getDouble(string &line) const;
  QString formatSpeed(const int speed) const;
  QString formatSize(const double bytes) const;

  double lasttime;
  Table_t table;
  QTableWidget *rec;
  QTableWidget *tra;
  QProgressBar *rdial;
  QProgressBar *tdial;
  QComboBox *combo;
};

#endif
