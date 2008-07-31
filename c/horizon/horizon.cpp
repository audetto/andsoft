#include <qapplication.h>
#include <qgrid.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qvbox.h>

#include <math.h>

#include "horizon.h"

Orizzonte::Orizzonte() 
    : QMainWindow(NULL, NULL, WDestructiveClose )
{
    setCaption("Orizzonte - (C)opyRight AndSoft Inc., 2002-2003");
    
    QVBox *vbox = new QVBox(this);
    
    QHBox *hbox = new QHBox(vbox);
    
    QGrid *griglia = new QGrid(2, hbox);
    
    QLabel *label;
    
    label = new QLabel("&Distanza (km):", griglia);
    distanza = new QLineEdit("20", griglia);
    label->setBuddy(distanza);
    
    label = new QLabel("&Altezza (m):", griglia);
    altezza = new QLineEdit("900", griglia);
    label->setBuddy(altezza);
    
    label = new QLabel("&Osservatore (m):", griglia);
    osservatore = new QLineEdit("2", griglia);
    label->setBuddy(osservatore);
    
    label = new QLabel("&Confronto (km):", griglia);
    confronto = new QLineEdit("10", griglia);
    label->setBuddy(confronto);
    
    label = new QLabel("&Raggio terrestre (m):", griglia);
    raggio = new QLineEdit("6371000", griglia);
    label->setBuddy(raggio);
    
    griglia = new QGrid(2, hbox);
    
    label = new QLabel("Quota all'orizzonte (m):", griglia);
    nascosta = new QLineEdit(griglia);
    nascosta->setReadOnly(true);
    nascosta->setAlignment(Qt::AlignRight);
    
    label = new QLabel("Altezza sull'orizzonte (m):", griglia);
    sporgenza = new QLineEdit(griglia);
    sporgenza->setReadOnly(true);
    sporgenza->setAlignment(Qt::AlignRight);
    
    label = new QLabel("Angolo sull'orizzonte:", griglia);
    angolo = new QLineEdit(griglia);
    angolo->setReadOnly(true);
    angolo->setAlignment(Qt::AlignRight);
    
    label = new QLabel("Orizzonte (km):", griglia);
    orizzonte = new QLineEdit(griglia);
    orizzonte->setReadOnly(true);
    orizzonte->setAlignment(Qt::AlignRight);
    
    label = new QLabel("Equivalente (m):", griglia);
    equivalente = new QLineEdit(griglia);
    equivalente->setReadOnly(true);
    equivalente->setAlignment(Qt::AlignRight);
    
    label = new QLabel("Note:", griglia);
    note = new QLineEdit(griglia);
    note->setReadOnly(true);
    note->setAlignment(Qt::AlignRight);
    
    QPushButton *qpb = new QPushButton("Go!", vbox);
    connect(qpb, SIGNAL(clicked()), this, SLOT(go()));
    
    setCentralWidget(vbox);
}

void Orizzonte::aboutQt()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void Orizzonte::go()
{
    bool ok;
    QString testo;
    
    double dist = distanza->text().toDouble(&ok) * 1000;
    double alte = altezza->text().toDouble(&ok);
    double osse = osservatore->text().toDouble(&ok);
    double ragg = raggio->text().toDouble(&ok);
    double conf = confronto->text().toDouble(&ok) * 1000;
    
    double oriz = acos(ragg / (ragg + osse)) * ragg;
    orizzonte->setText(testo.setNum(oriz / 1000, 'f', 3));
    
    double CC = -1;
    
    if (dist > oriz)
    {
	double resi = (dist - oriz) / ragg; // angolo tra l'orizzonte e l'oggetto
	double mini = ragg * (1 / cos(resi) - 1);
	nascosta->setText(testo.setNum(mini, 'f', 3));
	
	double c = alte - mini;
	if (c > 0)
	{
	    note->setText("oggetto parzialmente visibile");
	    sporgenza->setText(testo.setNum(c, 'f', 3));
	    
	    double dire = oriz / ragg; // angolo tra l'osservatore e l'orizzonte
	    double b    = ragg * (tan(resi) + tan(dire));
	    double t2AB = (b - c) / (b + c) * tan(0.5 * (0.5 * M_PI - resi)); // tang di 0.5 * (A - B)
	    
	    double AB   = 2 * atan(t2AB); // A + B
	    CC          = (0.5 * M_PI - resi - AB) * 0.5;
	    double C    = CC / (2 * M_PI) * 360;
	    angolo->setText(testo.setNum(C, 'f', 3));
	}
	else
	{
	    note->setText("oggetto nascosto");
	    sporgenza->setText("oggetto nascosto");
	    angolo->setText("oggetto nascosto");
	}
    }
    else
    {
	note->setText("oggetto interamente visibile");
	
	double dire = dist / ragg; // angolo tra l'osservatore e l'oggetto
	double resi = (oriz - dist) / ragg; // angolo tra l'oggetto e l'orizzonte
	
	double mini = ragg * (1 / cos(resi) - 1);
	nascosta->setText(testo.setNum(mini, 'f', 3));
	
	double c    = alte - mini;
	
	if (c > 0)
	{
	    sporgenza->setText(testo.setNum(c, 'f', 3));
	    
	    double a    = sqrt((ragg + osse) * (ragg + osse) + (ragg + alte) * (ragg + alte) - 
			       2 * cos(dire) * (ragg + osse) * (ragg + alte)); // teorema del coseno
	    
	    double sinC = sin(0.5 * M_PI - resi) * c / a;
	    CC          = asin(sinC);
	    double C    = CC / (2 * M_PI) * 360;
	    angolo->setText(testo.setNum(C, 'f', 3));
	    
	}
	else 
	{
	    sporgenza->setText("sotto l'orizzonte");
	    angolo->setText("sotto l'orizzonte");
	}
	
    }
    
    if (CC > 0)
    {
	if (conf > oriz)
	{
	    double resi = (conf - oriz) / ragg; // angolo tra il confronto e l'orizzonte
	    double mini = ragg * (1 / cos(resi) - 1);
	    
	    double dire = oriz / ragg; // angolo tra l'osservatore e l'orizzonte
	    double b    = ragg * (tan(resi) + tan(dire));
	    double B    = 0.5 * M_PI - CC - resi;
	    double c    = sin(CC) / sin(B) * b + mini; // teorema dei seni
	    
	    equivalente->setText(testo.setNum(c, 'f', 3));
	    
	}
	else 
	{
	    double dire = conf / ragg; // angolo tra l'osservatore e il confronto
	    double resi = (oriz - conf) / ragg; // angolo tra il confronto e l'orizzonte
	    double mini = ragg * (1 / cos(resi) - 1);
	    
	    double b    = sqrt((ragg + osse) * (ragg + osse) + (ragg + mini) * (ragg + mini) - 
			       2 * cos(dire) * (ragg + osse) * (ragg + mini));
	    double B    = 0.5 * M_PI - CC + resi;
	    double c    = sin(CC) / sin(B) * b + mini; // teorema dei seni
	    
	    equivalente->setText(testo.setNum(c, 'f', 3));
	}
    }
    else
    {
	equivalente->setText("oggetto nascosto");
    }
    
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);		// create application object
    Orizzonte *oriz = new Orizzonte();
    oriz->show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
