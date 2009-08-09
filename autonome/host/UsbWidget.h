#ifndef __USBWIDGET__H__
#define __USBWIDGET__H__

#include <usb.h>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTimer>

class UsbWidget : public QWidget {
Q_OBJECT
public:
    UsbWidget(QWidget *parent = NULL,bool autostart = true);
    ~UsbWidget();
public slots:
    void setLayer(int k);
    void setLeds(const unsigned char *state);
protected slots:
    void setIntensity(int k);
    void setRunning(bool on);
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
    QSlider *intensity;
    QTimer *pool_timer;
    QLabel *status;
    unsigned char *old_buffer;
};

#endif
