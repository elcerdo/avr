#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include "AutonomeWidget.h"
#include "SettingsWidget.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget *parent = NULL);
private:
    AutonomeWidget *autonome;
    SettingsWidget *settings;
};

#endif
