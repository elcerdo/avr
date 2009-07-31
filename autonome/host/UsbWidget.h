#ifndef __USBWIDGET__H__
#define __USBWIDGET__H__

#include <usb.h>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

class UsbWidget : public QWidget {
Q_OBJECT
public:
    UsbWidget(QWidget *parent = NULL,bool autostart = true);
    ~UsbWidget();
public slots:
    void setRunning(bool on);
    void setLeds(const unsigned char *state);
    void update(void);
signals:
    void padPressed(int i,int j,bool state);
protected:
    void deviceError();
    int findDevice();
    void setLed(bool);
private:
    usb_dev_handle *handle;
    QPushButton *button;
    QTimer *pool_timer;
    QLabel *status;
    unsigned char *old_buffer;
};

#endif
