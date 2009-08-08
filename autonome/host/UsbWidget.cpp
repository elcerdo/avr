#include "UsbWidget.h"

#include <QVBoxLayout>
#include "usbdrv/opendevice.h"
#include "../firmware/usbconfig.h"
#include "../firmware/requests.h"

static const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
static const char          vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};

UsbWidget::UsbWidget(QWidget *parent,bool autostart) : QWidget(parent), handle(NULL), old_buffer(NULL) {
    usb_init();

    setLayout(new QVBoxLayout(this));

    button = new QPushButton("Start Device",this);
    button->setCheckable(true);
    connect(button,SIGNAL( clicked(bool) ),this,SLOT( setRunning(bool) )); 
    layout()->addWidget(button);

    status = new QLabel("Device status",this);
    layout()->addWidget(status);

    pool_timer = new QTimer(this);
    connect(pool_timer,SIGNAL( timeout() ),this,SLOT( update() ));
    pool_timer->setInterval( 10 );

    dynamic_cast<QBoxLayout*>(layout())->addStretch(1);

    old_buffer = new unsigned char[8];
    for (int k=0; k<8; k++) old_buffer[k] = 0;

    if ( autostart ) {
        button->setChecked(true);
        setRunning(true);
    }
}

void UsbWidget::setRunning(bool on) {
    if ( not findDevice() ) { 
        status->setText("Device error");
        button->setChecked(false);
        pool_timer->stop();
        return;
    }

    if ( on ) {
        status->setText("Device started");
        setLed(true);
        pool_timer->start();
    } else {
        status->setText("Device stopped");
        setLed(false);
        pool_timer->stop();
    }
}


int UsbWidget::findDevice() {
    if ( handle ) return 1;

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

void UsbWidget::deviceError() {
    qDebug("[usb] %s",usb_strerror());
    usb_close(handle);
    handle = NULL;

    setRunning( false );
}

void UsbWidget::setLayer(int k) {
    if ( handle == NULL ) return;

    int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, CUSTOM_RQ_LEDS_SET_LAYER, k, 0, NULL, 0, 5000);
    if ( cnt < 0 ) deviceError();
}

void UsbWidget::setLed(bool on) {
    if ( handle == NULL ) return;

    int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, CUSTOM_RQ_LED_SET_STATUS, on, 0, NULL, 0, 5000);
    if ( cnt < 0 ) deviceError();
}

void UsbWidget::setLeds(const unsigned char *state) {
    Q_ASSERT( sizeof(state) == 8 );
    if ( handle == NULL or not pool_timer->isActive() ) return;

    int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, CUSTOM_RQ_LEDS_SET_STATUS, 0, 0, reinterpret_cast<char*>(const_cast<unsigned char*>(state)), 8, 5000);
    if ( cnt < 0 ) deviceError();
}

void UsbWidget::update() {
    if ( handle == NULL ) return;

    unsigned char *buffer = new unsigned char [8];
    int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, CUSTOM_RQ_KEY_GET_STATUS, 0, 0, reinterpret_cast<char*>(buffer), sizeof(buffer), 5000);
    if ( cnt < 0 ) deviceError();

    for (int k=0; k<8; k++) {
        if ( old_buffer[k] != buffer[k]) {
            for (int l=0; l<8; l++) {
                if ( (old_buffer[k] & (1<<l)) != (buffer[k] & (1<<l)) ) {
                    emit padPressed(k,l,(buffer[k] & (1<<l)) != 0);
                }
            }
        }
    }

    delete [] old_buffer;
    old_buffer = buffer;
}

UsbWidget::~UsbWidget() {
    setRunning( false );
    if ( handle ) usb_close(handle);

    delete [] old_buffer;
}

