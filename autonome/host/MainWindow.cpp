#include "MainWindow.h"

#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QTabWidget *tabs = new QTabWidget(this);

    autonome = new AutonomeWidget(tabs);
    tabs->addTab(autonome,"pad");

    settings = new SettingsWidget(tabs);
    tabs->addTab(settings,"settings");

    setCentralWidget(tabs);
    setMinimumSize(400,400);
}

