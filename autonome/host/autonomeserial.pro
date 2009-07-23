CONFIG += qt debug link_pkgconfig
PKGCONFIG += liblo libusb
SOURCES += AutonomeWidget.cpp UsbWidget.cpp OSCWidget.cpp SettingsWidget.cpp MainWindow.cpp
HEADERS += AutonomeWidget.h   UsbWidget.h   OSCWidget.h   SettingsWidget.h   MainWindow.h
SOURCES += usbdrv/opendevice.cpp
SOURCES += main.cpp
