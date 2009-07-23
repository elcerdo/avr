#include "UsbWidget.h"

#include <QVBoxLayout>
#include "usbdrv/opendevice.h"
#include "../firmware/usbconfig.h"
#include "../firmware/requests.h"

static const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
static const char          vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};

UsbWidget::UsbWidget(QWidget *parent) : QWidget(parent), handle(NULL) {
    usb_init();

    setLayout(new QVBoxLayout(this));

    button = new QPushButton("led",this);
    button->setCheckable(true);
    connect(button,SIGNAL( clicked(bool) ),this,SLOT( setLed(bool) )); 
    layout()->addWidget(button);

    dynamic_cast<QBoxLayout*>(layout())->addStretch(1);

    findDevice();

}

int UsbWidget::findDevice() {
    if ( handle != NULL ) return 1;

    int vid = rawVid[1] * 256 + rawVid[0];
    int pid = rawPid[1] * 256 + rawPid[0];

    if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0) {
        qDebug("[usb] could not find USB device \"%s\" with vid=0x%x pid=0x%x", product, vid, pid);
        handle = NULL;
        return 0;
    }

    qDebug("[usb] found device");
    return 1;
}

void UsbWidget::setLed(bool on) {
    if ( not findDevice() ) return;

    char buffer[4];
    int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, CUSTOM_RQ_SET_STATUS, on, 0, buffer, 0, 5000);

    if ( cnt < 0 ) {
        qDebug("[usb] %s",usb_strerror());
        usb_close(handle);
        handle = NULL;
    }
}

UsbWidget::~UsbWidget() {
    if ( handle ) usb_close(handle);
}

