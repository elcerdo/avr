#include "MainWindow.h"

#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QTabWidget *tabs = new QTabWidget(this);

    autonome = new AutonomeWidget(tabs);
    settings = new SettingsWidget(tabs);
    osc = new OSCWidget(tabs);

    connect(osc,SIGNAL( setLed(int,int,bool) ),autonome,SLOT( setLed(int,int,bool) ));
    connect(osc,SIGNAL( setLedColumn(int,int) ),autonome,SLOT( setLedColumn(int,int) ));
    connect(osc,SIGNAL( setLedRow(int,int) ),autonome,SLOT( setLedRow(int,int) ));
    connect(osc,SIGNAL( clearLed() ),autonome,SLOT( clearLed() ));
    connect(autonome,SIGNAL( padPressed(int,int,bool) ),osc,SLOT( padPressed(int,int,bool) ));

    tabs->addTab(autonome,"pad");
    tabs->addTab(osc,"osc");
    tabs->addTab(settings,"settings");

    setCentralWidget(tabs);
    setMinimumSize(400,400);
}

