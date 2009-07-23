#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include "AutonomeWidget.h"
#include "SettingsWidget.h"
#include "OSCWidget.h"
#include "UsbWidget.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget *parent = NULL);
private:
    OSCWidget *osc;
    AutonomeWidget *autonome;
    SettingsWidget *settings;
    UsbWidget *usb;
};

#endif
