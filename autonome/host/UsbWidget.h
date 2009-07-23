#ifndef __USBWIDGET__H__
#define __USBWIDGET__H__

#include <QWidget>
#include <usb.h>
#include <QPushButton>

class UsbWidget : public QWidget {
Q_OBJECT
public:
    UsbWidget(QWidget *parent = NULL);
    ~UsbWidget();
public slots:
    void setLed(bool on);
protected:
    int findDevice();
private:
    usb_dev_handle *handle;
    QPushButton *button;
};

#endif
