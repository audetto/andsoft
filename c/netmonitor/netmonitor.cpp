#include <stdlib.h>
#include <QApplication>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTimer>

#include <fstream>

#include <sys/time.h>

#include "netmonitor.h"

NetMonitor::NetMonitor() 
    : QMainWindow(NULL, NULL)
{
    timeval tv;
    gettimeofday(&tv, NULL);
    
    lasttime = tv.tv_sec;
    
    setWindowTitle("Net Monitor - (C)opyRight AndSoft Inc., 2003-08");
    
    QTabWidget *qt = new QTabWidget(this);
    
    QStringList columnNames;
    columnNames.append("Bytes");
    columnNames.append("Average");
    columnNames.append("Speed");
    columnNames.append("Maximum");
    
    rec = new QTableWidget();
    rec->setColumnCount(4);
    rec->setHorizontalHeaderLabels(columnNames);
    
    qt->addTab(rec, "Receive");
    
    tra = new QTableWidget();
    tra->setColumnCount(4);
    tra->setHorizontalHeaderLabels(columnNames);
    
    qt->addTab(tra, "Transmit");
    
    QWidget *vbox = new QWidget();
    
    QVBoxLayout *layout = new QVBoxLayout;
    combo = new QComboBox(vbox);
    combo->setEditable(FALSE);
    
    rdial = new QProgressBar(vbox);
    tdial = new QProgressBar(vbox);
    
    layout->addWidget(combo);
    layout->addWidget(rdial);
    layout->addWidget(tdial);
    vbox->setLayout(layout);
    
    qt->addTab(vbox, "Dial");
    
    setCentralWidget(qt);
    
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(FALSE);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(2000);
}

NetMonitor::~NetMonitor()
{
    for (Table_t::iterator it = table.begin(); it != table.end(); table.erase(it++))
    {
	InterfaceData* interface = it->second;
	delete interface;
    }
}

double NetMonitor::getDouble(string &line) const
{
    static const string CIFRE("0123456789");
    line.erase(0, line.find_first_of(CIFRE));
    const int a = line.find_first_not_of(CIFRE);
    string temp = line.substr(0, a);
    line.erase(0, a);
    return atof(temp.c_str());
}

void NetMonitor::extract(string &line, QString &name, double &rbytes, double &tbytes) const
{
    const QString value = line.substr(0, 6).c_str();
    name = value.simplified();
    line.erase(0, 7);
    
    rbytes = getDouble(line);
    getDouble(line);
    getDouble(line);
    getDouble(line);
    getDouble(line);
    getDouble(line);
    getDouble(line);
    getDouble(line);
    tbytes = getDouble(line);
}

QString NetMonitor::formatSpeed(const int speed) const
{
    QString s;
    static const double KB = 1024.0;
    static const double MB = 1024.0 * 1024.0;
    
    if (speed < KB)
	s.sprintf("%.2f B/s", double(speed));
    else
	if (speed < MB)
	    s.sprintf("%.2f kB/s", speed / KB);
	else
	    s.sprintf("%.2f MB/s", speed / MB);
    
    return s;
}

QString NetMonitor::formatSize(const double bytes) const
{
    QString s;
    static const double KB = 1024.0;
    static const double MB = 1024.0 * 1024.0;
    static const double GB = 1024.0 * 1024.0 * 1024.0;
    
    if (bytes < MB)
	s.sprintf("%.2f kB", bytes / KB);
    else
	if (bytes < GB)
	    s.sprintf("%.2f MB", bytes / MB);
	else
	    s.sprintf("%.2f GB", bytes / GB);
    
    return s;
}

void NetMonitor::update()
{
    ifstream dev("/proc/net/dev");
    char buf[255];
    
    timeval tv;
    gettimeofday(&tv, NULL);
    
    const double actualtime = tv.tv_sec + tv.tv_usec / 1000000.0;
    
    for (Table_t::iterator it = table.begin(); it != table.end(); it++)
    {
	InterfaceData* interface = it->second;
	interface->active = false;
    }
    
    dev.getline(buf, 250);
    dev.getline(buf, 250);
    
    while (dev.good() && !dev.eof())
    {
	dev.getline(buf, 250);
	string line(buf);
	QString selezionato = combo->currentText();
	
	if (line.length() > 50)
	{
	    QString name;
	    double rbytes, tbytes;
	    extract(line, name, rbytes, tbytes);
	    Table_t::iterator it = table.find(name);
	    InterfaceData *interface;
	    if (it != table.end())
	    {
		interface = it->second;
		const double ett = actualtime - interface->timestart;
		if (ett > 0)
		{
		    const int raverage = int((rbytes - interface->rec.start) / ett);
		    const int taverage = int((tbytes - interface->tra.start) / ett);
		    
		    rec->setItem(interface->row, 1, new QTableWidgetItem(formatSpeed(raverage)));
		    tra->setItem(interface->row, 1, new QTableWidgetItem(formatSpeed(taverage)));
		}
		const double elt = actualtime - lasttime;
		if (elt > 0)
		{
		    const int rspeed = int((rbytes - interface->rec.last) / elt);
		    const int tspeed = int((tbytes - interface->tra.last) / elt);
		    
		    rec->setItem(interface->row, 2, new QTableWidgetItem(formatSpeed(rspeed)));
		    tra->setItem(interface->row, 2, new QTableWidgetItem(formatSpeed(tspeed)));
		    
		    if (rspeed > interface->rec.maximum)
		    {
			rec->setItem(interface->row, 3, new QTableWidgetItem(formatSpeed(rspeed)));
			interface->rec.maximum = rspeed;
		    }
		    if (tspeed > interface->tra.maximum)
		    {
			tra->setItem(interface->row, 3, new QTableWidgetItem(formatSpeed(tspeed)));
			interface->tra.maximum = tspeed;
		    }
		    if (name == selezionato)
		    {
			rdial->setMaximum(interface->rec.maximum);
			tdial->setMaximum(interface->tra.maximum);
			rdial->setValue(rspeed);
			tdial->setValue(tspeed);
		    }
		}
	    }
	    else
	    {
		combo->addItem(name);
		
		interface = new InterfaceData;
		interface->row = table.size();
		
		rec->setRowCount(interface->row + 1);
		rec->setVerticalHeaderItem(interface->row, new QTableWidgetItem(name));
		
		tra->setRowCount(interface->row + 1);
		tra->setVerticalHeaderItem(interface->row, new QTableWidgetItem(name));
		
		interface->rec.start = rbytes;
		interface->tra.start = tbytes;
		interface->rec.maximum = 0;
		interface->tra.maximum = 0;
		interface->timestart = actualtime;
		table[name] = interface;
	    }
	    interface->rec.last = rbytes;
	    interface->tra.last = tbytes;
	    interface->active = true;
	    rec->setItem(interface->row, 0, new QTableWidgetItem(formatSize(rbytes)));
	    tra->setItem(interface->row, 0, new QTableWidgetItem(formatSize(tbytes)));
	}
    }
    
    for (Table_t::iterator it = table.begin(); it != table.end(); it++)
    {
	InterfaceData* interface = it->second;
	if (!interface->active)
	{
	    delete interface;
	    table.erase(it);
	}
    }
    
    lasttime = actualtime;
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);		// create application object
    QWidget *nm = new NetMonitor;
    nm->show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
